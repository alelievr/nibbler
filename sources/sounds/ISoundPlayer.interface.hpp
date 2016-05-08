/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISoundPlayer.interface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 01:21:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 17:39:24 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum class		SOUND
{
	FOOD,
	POOP,
	JOIN,
	LEFT,
	GAMEOVER,
	DEATH,
//	START
};

class ISoundPlayer
{
	public:
		
		virtual void	playSound(const char *filename) = 0;
		virtual void	playSound(const SOUND & s) = 0;
		virtual void	playBackground(void) = 0;
		virtual void	stopBackground(void) = 0;
};

typedef ISoundPlayer	*(*createSoundPlayerF)(void);
typedef void			(*deleteSoundPlayerF)(ISoundPlayer *s);
