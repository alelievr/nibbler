/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISlave.interface.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 19:58:37 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 20:34:39 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <queue>
#include <deque>

struct Point
{
	std::size_t		x;
	std::size_t		y;
}

struct Item
{
	enum class TYPE
	{
		FOOD,
		POOP
	}
	Point			coo;
	TYPE			type;
}

typedef std::queue<Point>	Points;
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
}

enum class EVENT
{
	LOSE,
	END
}

struct ISlave
{
	virtual ~ISlave(void) {};

	virtual bool
	open(std::size_t width, std::size_t height, std::string && name) = 0;

	virtual void
	getEvent(KEY & key) const = 0;

	virtual void
	render(Points const & snake, Items const & items, bool pause) const = 0;

	virtual void
	close(EVENT event);
}
