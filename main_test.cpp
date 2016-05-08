/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:22:36 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 17:50:27 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ISlave.interface.hpp"
#include "IServotron.interface.hpp"
#include "ISoundPlayer.interface.hpp"
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <deque>

void		startSound(void)
{
	static ISoundPlayer		*sp;
	void				*handler;
	createSoundPlayerF	csp;
	deleteSoundPlayerF	dsp;

	if (!(handler = dlopen("sound.so", RTLD_LAZY | RTLD_LOCAL)))
		exit(printf("%s\n", dlerror()));
	if (!(csp = (createSoundPlayerF)dlsym(handler, "createSoundPlayer")))
		exit(printf("%s\n", dlerror()));
	if (!(dsp = (deleteSoundPlayerF)dlsym(handler, "deleteSoundPlayer")))
		exit(printf("%s\n", dlerror()));

	sp = csp();
	sp->playBackground();
}

void		startServotron(void)
{
	static IServotron	*servo;
	void				*servoHandler;
	createServotronF	cServo;
	deleteServotronF	dServo;

	if (!(servoHandler = dlopen("./servotron.so", RTLD_LAZY | RTLD_LOCAL)))
		exit(printf("%s\n", dlerror()));
	if (!(cServo = (createServotronF)dlsym(servoHandler, "createServotron")))
		exit(printf("%s\n", dlerror()));
	if (!(dServo = (deleteServotronF)dlsym(servoHandler, "deleteServotron")))
		exit(printf("%s\n", dlerror()));

	servo = cServo();
//	servo->startServer();
}

int			main(__attribute__((unused)) int ac, char **av)
{
	void		*handler;
	createGUI_f	cgui;
	deleteGUI_f	dgui;
	Points		snake;
	Items		items;
	KEY			ev;
	ISlave		*gui;

	if (!(handler = dlopen(av[1], RTLD_LAZY | RTLD_LOCAL)))
		exit(printf("%s\n", dlerror()));
	if (!(cgui = (createGUI_f)dlsym(handler, "createGUI")))
		exit(printf("%s\n", dlerror()));
	if (!(dgui = (deleteGUI_f)dlsym(handler, "deleteGUI")))
		exit(printf("%s\n", dlerror()));

	gui = cgui();
	if (!gui->open(10, 10, "olol"))
		exit(printf("an error occured during window opening !\n"));

	startSound();
	startServotron();

	snake.push_front({0, 0});
	snake.push_front({1, 0});
	snake.push_front({2, 0});
	items.push_front({{2, 2}, Item::TYPE::FOOD});
	while (42)
	{
		gui->render(snake, items, false, true);
		gui->getEvent(ev);
		if (ev == KEY::ESCAPE)
			exit(0);
		if (ev == KEY::LEFT)
			std::cout << "left\n";
		usleep(1000);
	}
	dgui(gui);
	return (0);
}
