/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoundPlayer.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 17:27:04 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 18:13:00 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SoundPlayer.class.hpp"
#include <unistd.h>

SoundPlayer::SoundPlayer(void) :
/*	_soundList[0]("assets/sounds/coin.ogg"),
	_soundList[1]("assets/sounds/death.ogg"),
	_soundList[2]("assets/sounds/coin.ogg"),
	_soundList[3]("assets/sounds/door_open.ogg"),
	_soundList[4]("assets/sounds/door_close.ogg"),*/
	_background("assets/sounds/arcade_loop.ogg")
{
	std::cout << "Default constructor of SoundPlayer called" << std::endl;
}

SoundPlayer::~SoundPlayer(void)
{
	std::cout << "Destructor of SoundPlayer called" << std::endl;
}

void			SoundPlayer::playSound(const char *filename)
{
	for (int i = 0; i < 32; i++)
		if (!_soundList[i].isPlaying())
		{
			_soundList[i](filename);
			_soundList[i].play();
			break ;
		}

}

void			SoundPlayer::playSound(const SOUND & s)
{
	this->playSound(soundMap[s].c_str());
}

void			SoundPlayer::playBackground(void)
{
	_background.setLoop(true);
	_background.play();
}

void			SoundPlayer::stopBackground(void)
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

	sp.playSound(SOUND::FOOD);
	sp.playBackground();
	while (42);
	return (0);
}
