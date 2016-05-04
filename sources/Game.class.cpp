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

#include <iostream>
Game::Game(int argc, char **argv) :
	Main(argc, argv),
	_dir(DIRECTION::LEFT)
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
}

int
Game::run(void)
{
	KEY		key;

	this->getGUI(_args[3]);
	if (not _gui->open(_width, _height, "olol"))
		throw Exception("Cannot open the window");
	_timer.frame(30);
	while (42)
	{
		if (_timer.frame(30))
		{
			_gui->render(_snake, _items, false);
		}
		_gui->getEvent(key);
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
				// TODO
				break ;
			case KEY::TWO:
				// TODO
				break ;
			case KEY::THREE:
				// TODO
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
		// check the head (is it inside a wall or inside the snake ?)
		_snake.push_back(Point{x, y});
		// manage food && bonus
	}
	// never reached
	return 0;
}
