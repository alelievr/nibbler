#include "SoundPlayer.class.hpp"


SoundPlayer::SoundPlayer(void) :
	_eat("assets/sounds/coin.ogg"),
	_die("assets/sounds/death.ogg"),
	_poop("assets/sounds/coin.ogg"),
	_background("assets/sounds/arcade_loop.ogg"),
	_joinGame("assets/sounds/door_open.ogg"),
	_leftGame("assets/sounds/door_close.ogg")
{
	std::cout << "Default constructor of SoundPlayer called" << std::endl;
}

SoundPlayer::~SoundPlayer(void)
{
	std::cout << "Destructor of SoundPlayer called" << std::endl;
}

void			SoundPlayer::playSound(char *filename)
{
	SimpleSound	ss(filename);

	ss.play();
}

void			SoundPlayer::playEatSound(void)
{
	_eat.play();
}

void			SoundPlayer::playDieSound(void)
{
	_die.play();
}

void			SoundPlayer::playPoopSound(void)
{
	_poop.play();
}

void			SoundPlayer::playBackgroundSound(void)
{
	_background.setLoop(true);
	_background.play();
}

void			SoundPlayer::stopBackgroundSound(void)
{
	_background.pause();
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

int			main(void)
{
	SoundPlayer	sp;

	sp.playEatSound();
	sp.playBackgroundSound();
	while (42);
	return (0);
}
