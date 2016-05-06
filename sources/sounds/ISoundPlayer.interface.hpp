/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISoundPlayer.interface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 01:21:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/07 01:31:06 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class ISoundPlayer
{
	public:
		
		virtual void	playSound(char *filename) const = 0;
		virtual void	playEatSound(void) const = 0;
		virtual void	playDieSound(void) const = 0;
		virtual void	playPoopSound(void) const = 0;
		virtual void	playBackgroundSound(void) const = 0;
};

typedef ISoundPlayer	*(*createSoundPlayerF)(void);
typedef void			(*deleteSoundPlayerF)(ISoundPlayer *s);
