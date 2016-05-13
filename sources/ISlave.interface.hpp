/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISlave.interface.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 19:58:37 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/05/14 00:43:46 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <deque>
#include <iostream>
#include <map>

struct Point
{	/* Origin: Up && Left */
	std::size_t		x;
	std::size_t		y;
	inline bool operator==(Point const & p) { return (x == p.x && y == p.y); }
};


struct Item
{
	enum class TYPE
	{
		FOOD,
		POOP,
		WALL
	};
	Point			coo;
	TYPE			type;
};

typedef std::deque< Point >			Points;
typedef std::deque< Item >			Items;
typedef int							Client;

enum class DIRECTION
{
	UP, DOWN, LEFT, RIGHT
};

struct Player
{
	Points		snake;
	DIRECTION	dir;
	bool		paused;
};

typedef std::map< Client, Player >	Players;


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
	ENTER,
	ESCAPE,
	JOINSEVER,
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
	render(Players const & snake, Items const & items, bool pause, bool start, std::deque< std::string > const & ipList) = 0;

	virtual void
	getClickedIp(std::string & ip) const = 0;

	virtual void
	close(EVENT event) = 0;
};

typedef ISlave	*(*createGUI_f)(void);
typedef void	(*deleteGUI_f)(ISlave *);
