/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoundPlayer.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 17:26:59 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 17:47:52 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOUNDPLAYER_HPP
# define SOUNDPLAYER_HPP
# include <iostream>
# include <string>
# include <map>
# include "ISoundPlayer.interface.hpp"
# include "SFML/Audio.hpp"
# include "SimpleSound.class.hpp"

std::map< SOUND, std::string > soundMap = {
	{SOUND::FOOD, "assets/sounds/coin.ogg"},
	{SOUND::POOP, "assets/sounds/coin.ogg"}, //TODO: change this sound
	{SOUND::JOIN, "assets/sounds/door_open.ogg"},
	{SOUND::LEFT, "assets/sounds/door_close.ogg"},
	{SOUND::GAMEOVER, "assets/sounds/coin.ogg"}, //TODO change also
	{SOUND::DEATH, "assets/sounds/death.ogg"},
};

class		SoundPlayer : ISoundPlayer
{
	private:
		SimpleSound	_soundList[32];
		SimpleSound	_background;

		void	payOneShot(sf::Sound const & sound, float pitch = 1.0f, float volume = 75.0f) const;

	public:
		SoundPlayer();
		SoundPlayer(const SoundPlayer &) = delete;
		virtual ~SoundPlayer(void);

		void	playSound(const char *filename);
		void	playSound(const SOUND & s);
		void	playBackground(void);
		void	stopBackground(void);

		SoundPlayer &	operator=(SoundPlayer const & src) = delete;
};

std::ostream &	operator<<(std::ostream & o, SoundPlayer const & r);

extern "C" {
	SoundPlayer	*createSoundPlayer(void);
	void		deleteSoundPlayer(SoundPlayer *s);
}

#endif
