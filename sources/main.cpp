/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:00:11 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:37:27 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Main.class.hpp"

#include <iostream>

int
main(int argc, char **argv)
{
	Main		prog(argc, argv);

	try
	{
		return prog.run();
	}
	catch (Exception const & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return -1;
	}
}
