#pragma once

#include "Main.class.hpp"
#include "ISlave.interface.hpp"
#include "Timer.class.hpp"

class Game : public Main
{
	enum class DIRECTION
	{
		UP, DOWN, LEFT, RIGHT
	};
	public:
		Game(int argc, char **argv);
		~Game(void) = default;
		Game(Game const &) = delete;
		Game & operator=(Game const &) = delete;

		int
		run(void);

	private:
		void
		getGUI(std::string const & libname);

		Points				_snake;
		Items				_items;
		DIRECTION			_dir;
		Timer				_timer;
		bool				_started;
		bool				_paused;

		std::string	const	_title;

		// Dynamic UI
		createGUI_f			_create_gui;
		deleteGUI_f			_delete_gui;
		ISlave				*_gui;

		std::size_t					_active_ui;
};
