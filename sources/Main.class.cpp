/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:21:01 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:47:32 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Main.class.hpp"

Main::Main(int argc, char **argv)
{
	arguments(argc, argv);
}

int
Main::run(void)
{
	// main loop
	(void)_width, (void)_height;
	return 0;
}

void
Main::arguments(int argc, char **argv)
{
	(void)argc, (void)argv;
	// handle arguments
	// throw if error
	throw Exception(ERROR::BAD_ARGS);
}
