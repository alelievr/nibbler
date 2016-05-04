#include "Game.class.hpp"

#include <dlfcn.h>

static inline bool
is_in_snake(std::size_t x, std::size_t y, Points & snake)
{
	for (auto & p : snake)
		if (p.x == x and p.y == y)
			return true;
	return false;
}

Game::Game(int argc, char **argv) :
	Main(argc, argv),
	_dir(DIRECTION::LEFT),
	_title("Olol nibble"),
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

int
Game::run(void)
{
	KEY					key;

	this->getGUI(_args[3]);
	while (42)
	{
		if (_timer.frame(30))
		{
			_gui->render(_snake, _items, false);
		}
		_gui->getEvent(key);
		std::size_t	n;
		switch (key)
		{
			case KEY::LEFT:
				if (_dir != DIRECTION::RIGHT)
					_dir = DIRECTION::LEFT;
				break ;
			case KEY::RIGHT:
				if (_dir != DIRECTION::LEFT)
					_dir = DIRECTION::RIGHT;
				break ;
			case KEY::UP:
				if (_dir != DIRECTION::DOWN)
					_dir = DIRECTION::UP;
				break ;
			case KEY::DOWN:
				if (_dir != DIRECTION::UP)
					_dir = DIRECTION::DOWN;
				break ;
			case KEY::PAUSE:
				_gui->render(_snake, _items, true);
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
			case KEY::NONE: break ;
		}
		std::size_t		x(_snake.back().x);
		std::size_t		y(_snake.back().y);
		_snake.pop_front();
		switch (_dir)
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
		if (x >= _width or y >= _height or is_in_snake(x, y, _snake))
			return _gui->close(EVENT::GAMEOVER), 0;
		_snake.push_back(Point{x, y});
		// manage food && bonus
	}
	// never reached
	return 0;
}
