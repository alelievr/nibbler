/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSound.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 17:26:58 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 17:55:41 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SFML/Audio.hpp"

class		SimpleSound
{
	private:
		sf::SoundBuffer	_buff;
		bool			_ok;

	public:
		sf::Sound		sound;

		SimpleSound(const char * filename);
		SimpleSound(void);
		SimpleSound(std::string const & filename);
		SimpleSound(const SimpleSound & s);

		void				setVolume(float f);
		void				setPitch(float f);
		void				setLoop(bool b);
		void				play(void);
		void				pause(void);
		void				stop(void);
		bool				isPlaying(void) const;
		sf::SoundBuffer const &	getBuffer(void) const;
		bool				getOK(void) const;

		void				operator()(const char *filename);
};
