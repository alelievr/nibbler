#include "Game.class.hpp"

#include <dlfcn.h>
#include <unistd.h>

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
	_started(false),
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

    _servo = _create_servo();
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
Game::moveOnePlayer(Client const & c)
{
	DIRECTION &	dir		= _players[c].dir;
	Points &	snake	= _players[c].snake;
	KEY			key;

	_servo->getClientEvent(c, key);
	switch (key)
	{
		case KEY::LEFT:
			if (dir != DIRECTION::RIGHT)
				dir = DIRECTION::LEFT;
			break ;
		case KEY::RIGHT:
			if (dir != DIRECTION::LEFT)
				dir = DIRECTION::RIGHT;
			break ;
		case KEY::UP:
			if (dir != DIRECTION::DOWN)
				dir = DIRECTION::UP;
			break ;
		case KEY::DOWN:
			if (dir != DIRECTION::UP)
				dir = DIRECTION::DOWN;
			break ;
		case KEY::ENTER:
			if (_players[c].paused)
				_players[c].paused = false;
			break ;
		case KEY::PAUSE:
				_players[c].paused = !_players[c].paused;
				break ;
		case KEY::ONE:
		case KEY::TWO:
		case KEY::THREE:
		case KEY::ESCAPE:
		case KEY::JOINSEVER:
		case KEY::NONE: break ;
	}
	if (!_players[c].paused)
	{
		std::size_t		x(snake.back().x);
		std::size_t		y(snake.back().y);
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
		snake.push_back(Point{x, y});
	}
}

bool
Game::moveMe(KEY const & key)
{
	DIRECTION &		dir = _players[0].dir;
	Points &		snake = _players[0].snake;
	std::size_t		n;

	switch (key)
	{
		case KEY::LEFT:
			if (dir != DIRECTION::RIGHT)
				dir = DIRECTION::LEFT;
			break ;
		case KEY::RIGHT:
			if (dir != DIRECTION::LEFT)
				dir = DIRECTION::RIGHT;
			break ;
		case KEY::UP:
			if (dir != DIRECTION::DOWN)
				dir = DIRECTION::UP;
			break ;
		case KEY::DOWN:
			if (dir != DIRECTION::UP)
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
			_started = true;
			break ;
		case KEY::JOINSEVER:
			break ;
		case KEY::NONE: break ;
	}
	if (_started && !_paused)
	{
		std::size_t		x(snake.back().x);
		std::size_t		y(snake.back().y);
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
		if (x >= _width or y >= _height or is_in_snake(x, y, snake))
			return _gui->close(EVENT::GAMEOVER), 0;
		snake.push_back(Point{x, y});
	}
	return 1;
}

Points	newSnake(std::size_t w, std::size_t h) {
	std::size_t		x(w / 2);
	std::size_t		y(h / 2);
	Points			s;

	s.push_back({x, y});
	s.push_back({x + 1, y});
	s.push_back({x + 1, y + 1});
	s.push_back({x, y + 1});
	return (s);
}

int
Game::run(void)
{
	static KEY					lastKey = KEY::NONE;
	KEY							key;
	STATE						state;
	std::deque< std::string >	ipList;
	std::string					clickedIp;

	//		TODO:
	//do a list of players and send if to the clients
	//
	//add the sound: play a sound with _sp->playSound(SOUND const & s);

	this->getGUI(_args[3]);
	this->getServo();
	this->getSoundPlayer();
	while (42)
	{
		//timer causing input lag if > 0
		if (_timer.frame(0))
			_gui->render(_players, _items, _paused, _started, ipList);

		_gui->getEvent(key);
		_servo->getState(state);
		_gui->getClickedIp(clickedIp);
		_servo->getOnlineIpList(ipList);
		if (clickedIp.compare(""))
			_servo->connectToServer(clickedIp);

		if (state == STATE::CLIENT && key != KEY::NONE && key != lastKey)
			_servo->sendEvent(key);
		else
		{
			if (key != KEY::NONE && key != lastKey)
			_servo->sendEventToClients(key);
			_servo->getConnectedClients(_clients);
			for (auto const & c : _clients) {
				if (!(_players.count(c))) //index does not exists in map
					_players.insert(std::pair< Client, Player >(c, Player{newSnake(_width, _height), DIRECTION::LEFT, true}));
				moveOnePlayer(c);
			}
		}
		if (!moveMe(key))
			break ;
		lastKey = key;
		// manage food && bonus
	}
	_delete_gui(_gui);
	_delete_servo(_servo);
	_delete_soundPlayer(_sp);
	return 0;
}
