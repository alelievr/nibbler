#include "Game.class.hpp"

#include <dlfcn.h>
#include <unistd.h>
#include <algorithm>

__attribute((unused)) static inline bool
is_in_snake(std::size_t x, std::size_t y, Points & snake)
{
	for (auto & p : snake)
		if (p.x == x and p.y == y)
			return true;
	return false;
}

Game::Game(int argc, char **argv) :
	Main(argc, argv),
	_paused(false),
	_title("Olol nibble"),
	_snake(_players[0].snake),
	_active_ui(1ul)
{
	std::size_t		x(_width / 2);
	std::size_t		y(_width / 2);

	_snake.push_back({x, y});
	_snake.push_back({x + 1, y});
	_snake.push_back({x + 1, y + 1});
	_snake.push_back({x, y + 1});
}

void
Game::getGUI(std::string const & libname)
{
	void		*handler;

	if (not (handler = dlopen(libname.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		throw Exception(dlerror());
	if (not (_create_gui = (createGUI_f)dlsym(handler, "createGUI")))
		throw Exception(dlerror());
	if (not (_delete_gui = (deleteGUI_f)dlsym(handler, "deleteGUI")))
		throw Exception(dlerror());
	_gui = _create_gui();
	if (not _gui->open(_width, _height, _title))
		throw Exception("Cannot open the window");
}

void
Game::getServo(void)
{
    void                *servoHandler;

    if (!(servoHandler = dlopen("servotron.so", RTLD_LAZY | RTLD_LOCAL)))
		throw Exception(dlerror());
    if (!(_create_servo	= (createServotronF)dlsym(servoHandler, "createServotron")))
		throw Exception(dlerror());
    if (!(_delete_servo = (deleteServotronF)dlsym(servoHandler, "deleteServotron")))
		throw Exception(dlerror());

    _servo = _create_servo(_width, _height);
	me = _servo->getLocalId();
	auto const & it = _players.begin();
	std::swap(_players[me], it->second);
	_players.erase(0);
	//_servo->startServer();
}

void
Game::getSoundPlayer(void)
{
	void                *handler;

	if (!(handler = dlopen("sound.so", RTLD_LAZY | RTLD_LOCAL)))
		throw Exception(dlerror());
	if (!(_create_soundPlayer = (createSoundPlayerF)dlsym(handler, "createSoundPlayer")))
		throw Exception(dlerror());
	if (!(_delete_soundPlayer = (deleteSoundPlayerF)dlsym(handler, "deleteSoundPlayer")))
		throw Exception(dlerror());

	_sp = _create_soundPlayer();
	_sp->playBackground();
}

void
Game::respawnSnake(void)
{
	_players[me].started = false;
	std::size_t		x(_width / 2);
	std::size_t		y(_height / 2);

	for (auto i : _players[me].snake)
		_servo->popSnakeBlock(i);
	_players[me].snake.clear();
	_players[me].snake.push_back({x, y});
	_servo->addSnakeBlock({x, y});
	_players[me].snake.push_back({x + 1, y});
	_servo->addSnakeBlock({x + 1, y});
	_players[me].snake.push_back({x + 1, y + 1});
	_servo->addSnakeBlock({x + 1, y + 1});
	_players[me].snake.push_back({x, y + 1});
	_servo->addSnakeBlock({x, y + 1});
	_servo->updateClientState(CLIENT_BYTE::STARTED_BYTE, false);
	_players[me].started = false;
	_players[me].dead = false;
	_players[me].dir = DIRECTION::UP;
}

bool
Game::moveMe(KEY const & key)
{
	static KEY		lastKey = KEY::NONE;
	static DIRECTION lastDirection = DIRECTION::LEFT;	
	DIRECTION &		dir = _players[me].dir;
	Points &		snake = _players[me].snake;
	std::size_t		n;
	bool			moved = false;

	static clock_t		keyTime = 0;
	static clock_t		moveTime = 0;
	if (!moveTime)
		moveTime = clock();

	if (key != lastKey && clock())
	{
		switch (key)
		{
			case KEY::LEFT:
				if (lastDirection != DIRECTION::RIGHT)
					dir = DIRECTION::LEFT;
				break ;
			case KEY::RIGHT:
			if (lastDirection != DIRECTION::LEFT)
					dir = DIRECTION::RIGHT;
				break ;
			case KEY::UP:
				if (lastDirection != DIRECTION::DOWN)
					dir = DIRECTION::UP;
				break ;
			case KEY::DOWN:
				if (lastDirection != DIRECTION::UP)
					dir = DIRECTION::DOWN;
				break ;
			case KEY::PAUSE:
				_paused = !_paused;
				break ;
			case KEY::ONE:
			case KEY::TWO:
			case KEY::THREE:
				n = static_cast<std::size_t>(key)
					- static_cast<std::size_t>(KEY::ONE) + 1;
				if (_active_ui == n or _args.size() < 3 + n)
					break ;
				_gui->close(EVENT::SWITCH);
				this->getGUI(_args[2 + n]);
				_active_ui = n;
				break ;
			case KEY::ESCAPE:
				return _gui->close(EVENT::GAMEOVER), 0;
			case KEY::ENTER:
				if (_players[me].dead)
					this->respawnSnake();
				if (!_players[me].started)
				{
					_invincibleClock = clock();
					_players[me].invincible = true;
					_servo->updateClientState(CLIENT_BYTE::INVINCIBLE_BYTE, true);
					_servo->updateClientState(CLIENT_BYTE::STARTED_BYTE, true);
					_servo->updateClientState(CLIENT_BYTE::DEAD_BYTE, false);
				}
				_players[me].started = true;
				break ;
			case KEY::JOINSEVER:
				break ;
			case KEY::NONE: break ;
		}
		keyTime = clock();
	}
	if (_players[me].started && !_paused && clock() - moveTime > MOVE_TICKS && !_players[me].dead)
	{
		moveTime = clock();
		moved = true;
		std::size_t		x(snake.back().x);
		std::size_t		y(snake.back().y);
		Point			front(snake.front());
		_servo->popSnakeBlock(snake.front());
		snake.pop_front();
		switch (dir)
		{
			case DIRECTION::LEFT:
				--x; break ;
			case DIRECTION::RIGHT:
				++x; break ;
			case DIRECTION::UP:
				--y; break ;
			case DIRECTION::DOWN:
				++y; break ;
		}
		lastDirection = dir;
		if (x >= _width or y >= _height or is_in_snake(x, y, snake))
		{
			_players[me].dead = true;
			_servo->updateClientState(CLIENT_BYTE::DEAD_BYTE, true);
			return _gui->close(EVENT::GAMEOVER), 1;
		}
		snake.push_back(Point{x, y});
		_servo->addSnakeBlock(snake.back());

		//manage food && bonus
		for (auto const & i : _items)
			if (i.coo == snake.back())
			{
				switch (i.type)
				{
					case Item::TYPE::FOOD:
						snake.push_front(front);
						_sp->playSound(SOUND::FOOD);
						_servo->addSnakeBlock(front);
						break ;
					case Item::TYPE::POOP:
						_sp->playSound(SOUND::POOP);
						_servo->popSnakeBlock(snake.front());
						snake.pop_front();
						break ;
					case Item::TYPE::WALL:
						return _gui->close(EVENT::GAMEOVER), 0;
						_servo->updateClientState(CLIENT_BYTE::DEAD_BYTE, true);
						_players[me].dead = true;
						break ;
				}
				_servo->deleteItem(i);
				_items.erase(std::find(_items.begin(), _items.end(), i));
				break ;
			}

		//manage others snake
		if (_invincibleClock + INVINCIBLE_TICKS <= clock())
		{
			if (_players[me].invincible)
			{
				_players[me].invincible = false;
				_servo->updateClientState(CLIENT_BYTE::INVINCIBLE_BYTE, false);
			}
			for (auto const & p : _players)
			{
				if (p.first == me)
					continue ;
				for (auto const & s : p.second.snake)
				{
					if (snake.back() == s)
					{
						_players[me].dead = true;
						_servo->updateClientState(CLIENT_BYTE::DEAD_BYTE, true);
						return _gui->close(EVENT::GAMEOVER), 1;
					}
				}
			}
		}
	}
	lastKey = key;

	return 1;
}

void
Game::genItems(void)
{
	if (rand() % 200)
		return ;
	Item	i;

	i.coo = {rand() % _width, rand() % _height};
	i.type = Item::TYPE::FOOD;
	_servo->addItem(i);
}

int
Game::run(void)
{
	KEY							lastKey = KEY::NONE;
	std::string					lastClickedIP = "";
	KEY							key;
	STATE						state;
	std::deque< std::string >	ipList;
	std::string					clickedIp;
	Point						serverGridSize;

	srand(clock());
	this->getGUI(_args[3]);
	this->getServo();
	this->getSoundPlayer();
	while (42)
	{
		//timer causing input lag if > 0
		//if (_timer.frame(30))
			_gui->render(_players, _items, _paused, _players[me].started, ipList);

		_gui->getEvent(key);
		_servo->getState(state);
		_gui->getClickedIp(clickedIp);
		_servo->getOnlineIpList(ipList);

		//servotron GUI management
		if (clickedIp.compare("") && clickedIp.compare(lastClickedIP))
		{
			_servo->connectToServer(clickedIp);
			_sp->playSound(SOUND::JOIN);
			_servo->getServerInfos(serverGridSize);
			_gui->updateGridSize(serverGridSize);
			this->respawnSnake();
			_width = serverGridSize.x;
			_height = serverGridSize.y;
			
			for (auto it = _players.begin(); it != _players.end(); ++it)
				if (it->first != me)
					_players.erase(it->first);
		}

		//Clients and server management
		_servo->getConnectedClients(_clients);
		for (auto const & c : _clients)
		{
			if (!(_players.count(c))) //index does not exists in map
				_players.insert(std::pair< Client, Player >(c, Player{Points{0}, DIRECTION::LEFT}));
			else
			{
				std::map< Client, Player >::const_iterator	index;
				if (((index = std::find_if(_players.begin(), _players.end(),
									[this]
									(const std::pair< Client, Player > & tmp) -> bool
									{
									return !_players.count(tmp.first);
									}
									))) != _players.end())
					_players.erase(index);
			}
		}
		_servo->getPlayerInfo(_players);

		// Items add/delete management
		if (state == STATE::SERVER)
			genItems();
		_servo->getItems(_items);

		//Events
		if (!moveMe(key))
			break ;
		lastKey = key;
		lastClickedIP = clickedIp;
	}
	_delete_gui(_gui);
	_delete_servo(_servo);
	_delete_soundPlayer(_sp);
	return 0;
}
