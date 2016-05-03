/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISlave.interface.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 19:58:37 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/05/03 02:24:28 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <queue>
#include <deque>

struct Point
{	/* Origin: Up && Left */
	std::size_t		x;
	std::size_t		y;
};

struct Item
{
	enum class TYPE
	{
		FOOD,
		POOP
	};
	Point			coo;
	TYPE			type;
};

typedef std::deque<Point>	Points;
typedef std::deque<Item>	Items;

enum class KEY
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	PAUSE,
	ONE,
	TWO,
	THREE,
	ESCAPE
};

enum class EVENT
{
	GAMEOVER,
	SWITCH
};

struct ISlave
{
	virtual ~ISlave(void) {};

	virtual bool
	open(std::size_t width, std::size_t height, std::size_t mapSize, std::string && name) = 0;

	virtual void
	getEvent(KEY & key) const = 0;

	virtual void
	render(Points const & snake, Items const & items, bool pause) const = 0;

	virtual void
	close(EVENT event) = 0;
};

typedef ISlave	*(*createGUI_f)(void);
typedef void	(*deleteGUI_f)(ISlave *);
