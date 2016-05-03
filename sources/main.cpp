/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:00:11 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:49:09 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.class.hpp"

#include <iostream>

int
main(int argc, char **argv)
{
	try
	{
		return Game(argc, argv).run();
	}
	catch (std::exception const & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return -1;
	}
}
