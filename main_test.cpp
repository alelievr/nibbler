/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:22:36 by alelievr          #+#    #+#             */
/*   Updated: 2016/04/30 01:23:41 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ISlave.interface.hpp"
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <deque>

int			main(void)
{
	void		*handler;
	createGUI_f	cgui;
	deleteGUI_f	dgui;
	Points		snake;
	Items		items;
	KEY			ev;
	ISlave		*gui;

	if (!(handler = dlopen("GLFWnibbler.so", RTLD_LAZY | RTLD_LOCAL)))
		exit(printf("%s\n", dlerror()));
	if (!(cgui = (createGUI_f)dlsym(handler, "createGUI")))
		exit(printf("%s\n", dlerror()));
	if (!(dgui = (deleteGUI_f)dlsym(handler, "deleteGUI")))
		exit(printf("%s\n", dlerror()));
	
	gui = cgui();
	gui->open(500, 500, "olol");
	while (42)
	{
		gui->render(snake, items, false);
		gui->getEvent(ev);
		usleep(1000);
	}
	dgui(gui);
	return (0);
}
