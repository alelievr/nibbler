/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDL_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/24 22:52:44 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"

Point		SDL_gui::mouse = {0, 0};
std::map< int, KEY > keyMap = {
	{SDLK_UP, KEY::UP},
	{SDLK_DOWN, KEY::DOWN},
	{SDLK_LEFT, KEY::LEFT},
	{SDLK_RIGHT, KEY::RIGHT},
	{SDLK_p, KEY::PAUSE},
	{SDLK_ESCAPE, KEY::ESCAPE},
	{SDLK_0, KEY::ONE},
	{SDLK_1, KEY::TWO},
	{SDLK_2, KEY::THREE},
	{SDLK_RETURN, KEY::ENTER},
};

SDL_gui::SDL_gui(void)
{
}

SDL_gui::SDL_gui(SDL_gui const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

SDL_gui::~SDL_gui(void)
{
	std::cout << "Destructor of SDL_gui called" << std::endl;
}


SDL_gui &	SDL_gui::operator=(SDL_gui const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

bool	SDL_gui::open(std::size_t width, std::size_t height, std::string const & name)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Error while initializing opengl: " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winSize.x, winSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0) {
        std::cout << "Error while creating window " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if(glContext == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();

        return false;
    }

	GUI::open(width, height, winSize);
	return (true);
}

void	SDL_gui::getEvent(KEY & key) const
{
	SDL_Event	event;
	int			k;

	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				k = event.key.keysym.sym;
				if (keyMap.count(k))
					key = keyMap[k];
				break;
			case SDL_MOUSEMOTION:
				SDL_gui::mouse = {std::size_t(event.motion.x), std::size_t(event.motion.y)};
				break ;
			case SDL_MOUSEBUTTONDOWN:
				break ;
			case SDL_QUIT:
				key = KEY::ESCAPE;
				break;
			default:
				break;
		}
	}
}

void    SDL_gui::updateGridSize(Point const & gd)
{
	    GUI::updateGridSize(gd);
}

void	SDL_gui::render(Players const & players, Items const & items, bool pause, bool started, std::deque< std::string > const & ipList)
{
	GUI::render(players, items, pause, started);
	GUI::renderServotron(ipList);
/*	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
	//	SDL_RenderCopy(renderer, texture, NULL, &texture_rect); 

	for (auto & s : snake)
		drawRect(s, 0xFF00FF);*/

	//	SDL_UpdateWindowSurface(this->window);

	SDL_GL_SwapWindow(window);
}

void	SDL_gui::getClickedIp(std::string & ip) const
{
	ip = GUI::getLastClickedIp();
}

void	SDL_gui::close(EVENT event)
{
	if (event == EVENT::SWITCH)
	{
		GUI::close(event);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
		std::cout << "quitting\n";
	}
	(void)event;
}

std::ostream &	operator<<(std::ostream & o, SDL_gui const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	SDL_gui		*createGUI(void)
	{
		return new SDL_gui();
	}

	void		deleteGUI(SDL_gui *gui)
	{
		delete gui;
	}
}
