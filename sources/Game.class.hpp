#pragma once

#include "Main.class.hpp"
#include "ISlave.interface.hpp"
#include "IServotron.interface.hpp"
#include "ISoundPlayer.interface.hpp"
#include "Timer.class.hpp"

#define MOVE_TICKS			20000
#define INVINCIBLE_TICKS	3000000

class Game : public Main
{

	public:
		Game(int argc, char **argv);
		~Game(void) = default;
		Game(Game const &) = delete;
		Game & operator=(Game const &) = delete;

		int
		run(void);
		bool
		moveMe(KEY const & k);

	private:
		void
		getGUI(std::string const & libname);
		void
		getServo(void);
		void
		getSoundPlayer(void);
		void
		genItems(void);

		Client				me;
		Players				_players;
		Clients				_clients;
		Items				_items;
		Timer				_timer;
		bool				_started;
		bool				_paused;
		bool				_dead;

		clock_t				_invincibleClock;

		std::string	const	_title;
		Points &			_snake;

		// Dynamic UI
		createGUI_f			_create_gui;
		deleteGUI_f			_delete_gui;
		ISlave				*_gui;

		// Multiplayer and server GUI
		createServotronF	_create_servo;
		deleteServotronF	_delete_servo;
		IServotron			*_servo;

		// Music and sounds
		createSoundPlayerF	_create_soundPlayer;
		deleteSoundPlayerF	_delete_soundPlayer;
		ISoundPlayer		*_sp;

		std::size_t			_active_ui;
};
