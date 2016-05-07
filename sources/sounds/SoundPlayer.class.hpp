#ifndef SOUNDPLAYER_HPP
# define SOUNDPLAYER_HPP
# include <iostream>
# include <string>
# include "ISoundPlayer.interface.hpp"
# include "SFML/Audio.hpp"

class		SoundPlayer : ISoundPlayer
{
	private:
		class		SimpleSound
		{
			private:
				sf::SoundBuffer	_buff;
				bool			_ok;

			public:
				sf::Sound		sound;

				SimpleSound(const char * filename) {
					this->_ok = true;
					if (this->_buff.loadFromFile(filename)) {
						this->sound.setBuffer(this->_buff);
						this->sound.setVolume(75);
						this->sound.setPitch(1.0f);
						this->sound.setLoop(false);
						std::cout << "loaded sound " << filename << std::endl;
					} else
						this->_ok = false;
				}

				SimpleSound(std::string const & filename) : SimpleSound(filename.c_str()) {}

				void	setVolume(float f) { if (this->_ok) this->sound.setVolume(f); }
				void	setPitch(float f) { if (this->_ok) this->sound.setPitch(f); }
				void	setLoop(bool b) { if (this->_ok) this->sound.setLoop(b); }
				void	play(void) { if (this->_ok) { this->sound.play(); std::cout << "playing !\n";} }
				void	pause(void) { if (this->_ok) this->sound.pause(); }
		};
		SimpleSound	_eat;
		SimpleSound	_die;
		SimpleSound	_poop;
		SimpleSound	_background;
		SimpleSound	_joinGame;
		SimpleSound	_leftGame;

		void	payOneShot(sf::Sound const & sound, float pitch = 1.0f, float volume = 0.75f) const;

	public:
		SoundPlayer();
		SoundPlayer(const SoundPlayer &) = delete;
		virtual ~SoundPlayer(void);

		void	playSound(char *filename);
		void	playEatSound(void);
		void	playDieSound(void);
		void	playPoopSound(void);
		void	playBackgroundSound(void);
		void	stopBackgroundSound(void);

		SoundPlayer &	operator=(SoundPlayer const & src) = delete;
};

std::ostream &	operator<<(std::ostream & o, SoundPlayer const & r);

extern "C" {
	SoundPlayer	*createSoundPlayer(void);
	void		deleteSoundPlayer(SoundPlayer *s);
}

#endif
