/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constant.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 22:42:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/12 01:23:38 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

//added 400 w for servotron ui
# define WIN_W	1400
# define WIN_H	1000

enum class TEXTURE
{
	FOOD,
	POOP,
	WALL,
	START,
	GAMEOVER,
};
