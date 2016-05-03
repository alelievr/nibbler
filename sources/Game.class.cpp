#include "Game.class.hpp"

#include <dlfcn.h>

#include <iostream>
Game::Game(int argc, char **argv) :
	Main(argc, argv),
	_dir(DIRECTION::RIGHT)
{
	std::size_t		x(_width / 2);
	std::size_t		y(_width / 2);

	_snake.push({x, y});
	_snake.push({x + 1, y});
	_snake.push({x + 1, y + 1});
	_snake.push({x, y + 1});
}

void
Game::getGUI(std::string const & libname)
{
	void		*handler;

	if (not (handler = dlopen(libname.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		throw Exception(std::string("Cannot load ") + libname);
	if (not (_create_ui = dlsym(handler, "createGUI")))
		throw Exception(std::string("Cannot load `createGUI' in ") + libname);
	if (not (_delete_ui = dlsym(handler, "deleteGUI")))
		throw Exception(std::string("Cannot load `deleteGUI' in ") + libname);
	_gui = _create_gui();
}

int
Game::run(void)
{
	KEY		key;

	this->getGUI("GLFWnibbler.so");
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
		switch (_dir)
		{
			//move snake
			case DIRECTION::LEFT:
			case DIRECTION::RIGHT:
			case DIRECTION::UP:
			case DIRECTION::DOWN:
				break ;
		}
		// check the head (is it inside a wall or inside the snake ?)
		// manage food && bonus
	}
	// never reached
	return 0;
}
