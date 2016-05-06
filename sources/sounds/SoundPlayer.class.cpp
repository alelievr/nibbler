#include "SoundPlayer.class.hpp"


SoundPlayer::SoundPlayer(void)
{
	std::cout << "Default constructor of SoundPlayer called" << std::endl;
}

SoundPlayer::SoundPlayer(SoundPlayer const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

SoundPlayer::~SoundPlayer(void)
{
	std::cout << "Destructor of SoundPlayer called" << std::endl;
}


SoundPlayer &	SoundPlayer::operator=(SoundPlayer const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

void			SoundPlayer::playSound(char *filename) const
{
	(void)filename;
}

void			SoundPlayer::playEatSound(void) const
{

}

void			SoundPlayer::playDieSound(void) const
{

}

void			SoundPlayer::playPoopSound(void) const
{

}

void			SoundPlayer::playBackgroundSound(void) const
{

}

std::ostream &	operator<<(std::ostream & o, SoundPlayer const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	SoundPlayer	*createSoundPlayer(void) {
		return new SoundPlayer;
	}

	void		deleteSoundPlayer(SoundPlayer *s) {
		delete s;
	}
}
