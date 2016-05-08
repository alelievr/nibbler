/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSound.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 17:28:12 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 18:07:38 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleSound.class.hpp"
#include <iostream>

SimpleSound::SimpleSound(const char * filename)
{
	this->_ok = true;
	if (this->_buff.loadFromFile(filename)) {
		this->sound.setBuffer(this->_buff);
		this->sound.setVolume(75);
		this->sound.setPitch(1.0f);
		this->sound.setLoop(false);
	} else
		this->_ok = false;
}

SimpleSound::SimpleSound(void) : _ok(false) {}

SimpleSound::SimpleSound(std::string const & filename) : SimpleSound(filename.c_str()) {}

SimpleSound::SimpleSound(const SimpleSound & s)
{
	this->_ok = s.getOK();
	this->_buff = s.getBuffer();
	this->sound = s.sound;
}

sf::SoundBuffer const & SimpleSound::getBuffer(void) const { return this->_buff; }
bool	SimpleSound::getOK(void) const { return this->_ok; }

void	SimpleSound::operator()(const char *filename)
{
	this->_ok = true;
	if (this->_buff.loadFromFile(filename)) {
		this->sound.setBuffer(this->_buff);
		this->sound.setVolume(75);
		this->sound.setPitch(1.0f);
		this->sound.setLoop(false);
		std::cout << "loaded audio file !\n";
	} else
		this->_ok = false;
}

void	SimpleSound::setVolume(float f)
{
	if (this->_ok)
		this->sound.setVolume(f);
}

void	SimpleSound::setPitch(float f)
{
	if (this->_ok)
		this->sound.setPitch(f);
}

void	SimpleSound::setLoop(bool b)
{
	if (this->_ok)
		this->sound.setLoop(b);
}

void	SimpleSound::play(void)
{
	if (this->_ok)
		this->sound.play();
}

void	SimpleSound::pause(void)
{
	if (this->_ok)
		this->sound.pause();
}

void	SimpleSound::stop(void)
{
	if (this->_ok)
		this->sound.stop();
}

bool	SimpleSound::isPlaying(void) const
{
	return (this->_ok) ? (sound.getStatus() == sf::Sound::Status::Playing) : false;
}
