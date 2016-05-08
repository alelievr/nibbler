/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constant.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 22:42:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 18:31:20 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

# define WIN_W	1000
# define WIN_H	1000

enum class TEXTURE
{
	FOOD,
	POOP,
	WALL,
	START,
	GAMEOVER,
};

std::map< TEXTURE, std::string > textureMap = {
	{TEXTURE::FOOD, "assets/sprites/pizza.png"},
	{TEXTURE::POOP, "assets/sprites/pizza.png"}, //TODO change
	{TEXTURE::WALL, "assets/sprites/pizza.png"}, //TODO change
	{TEXTURE::START, "assets/sprites/start.png"},
	{TEXTURE::GAMEOVER, "assets/sprites/gameover.png"},
};

std::map< Item::TYPE, TEXTURE > itemTexMap= {
	{Item::TYPE::FOOD, TEXTURE::FOOD},
	{Item::TYPE::POOP, TEXTURE::POOP},
	{Item::TYPE::WALL, TEXTURE::WALL},
};
