/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISlave.interface.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 19:58:37 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/05/05 21:35:36 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <deque>
#include <iostream>

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
	ESCAPE,
	FIRST = NONE,
	LAST = ESCAPE
};

inline KEY operator++(KEY & x) { return x = (KEY)(std::underlying_type<KEY>::type(x) + 1); }
inline KEY operator++(KEY & x, int) { return x = (KEY)(std::underlying_type<KEY>::type(x) + 1); }
inline KEY operator*(KEY c) { return c; } 
inline KEY begin(KEY r) { return KEY::FIRST; (void)r; }
inline KEY end(KEY r) { KEY l=KEY::LAST; return l++; (void)r; }

enum class EVENT
{
	GAMEOVER,
	SWITCH
};

struct ISlave
{
	virtual ~ISlave(void) {};

	virtual bool
	open(std::size_t width, std::size_t height, std::string const & name) = 0;

	virtual void
	getEvent(KEY & key) const = 0;

	virtual void
	render(Points const & snake, Items const & items, bool pause) const = 0;

	virtual void
	close(EVENT event) = 0;
};

typedef ISlave	*(*createGUI_f)(void);
typedef void	(*deleteGUI_f)(ISlave *);
