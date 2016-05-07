/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISoundPlayer.interface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 01:21:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/07 16:14:21 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class ISoundPlayer
{
	public:
		
		virtual void	playSound(char *filename) = 0;
		virtual void	playEatSound(void) = 0;
		virtual void	playDieSound(void) = 0;
		virtual void	playPoopSound(void) = 0;
		virtual void	playBackgroundSound(void) = 0;
		virtual void	stopBackgroundSound(void) = 0;
};

typedef ISoundPlayer	*(*createSoundPlayerF)(void);
typedef void			(*deleteSoundPlayerF)(ISoundPlayer *s);
