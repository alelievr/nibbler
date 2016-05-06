#ifndef SOUNDPLAYER_HPP
# define SOUNDPLAYER_HPP
# include <iostream>
# include <string>
# include "ISoundPlayer.interface.hpp"

class		SoundPlayer : ISoundPlayer
{
	private:


	public:
		SoundPlayer();
		SoundPlayer(const SoundPlayer&);
		virtual ~SoundPlayer(void);

		void	playSound(char *filename) const;
		void	playEatSound(void) const;
		void	playDieSound(void) const;
		void	playPoopSound(void) const;
		void	playBackgroundSound(void) const;

		SoundPlayer &	operator=(SoundPlayer const & src);
};

std::ostream &	operator<<(std::ostream & o, SoundPlayer const & r);

extern "C" {
	SoundPlayer	*createSoundPlayer(void);
	void		deleteSoundPlayer(SoundPlayer *s);
}

#endif
