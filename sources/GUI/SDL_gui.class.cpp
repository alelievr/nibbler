/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDL_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/13 01:41:16 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"

int SDL_gui::pressedKey = 0;

/*static void error_callback(int error, const char* description)
{
	(void)error;
	fputs(description, stderr);
}

static void key_callback(SDLwindow* window, int key, int scancode, int action, int mods)
{
	if (keyMap.count(key))
	{
		if (action == 1)
			SDL_gui::pressedKey |= 1 << keyMap[key].first;
		else if (action == 0)
			SDL_gui::pressedKey &= ~(1 << keyMap[key].first);
	}
	std::cout << SDL_gui::pressedKey << std::endl;
	(void)scancode;
	(void)mods;
	(void)window;
}*/

SDL_gui::SDL_gui(void)
{
//	glfwSetErrorCallback(error_callback);
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
	SDL_Event		e = {0};

	SDL_WaitEvent(&e);
	if (e.type == SDL_QUIT)
		key = KEY::ESCAPE;
	if (e.type == SDL_KEYDOWN)
	{
	}
	key = KEY::NONE;
}

void	SDL_gui::render(Points const & snake, Items const & items, bool pause, bool started, std::deque< std::string > const & ipList)
{


	GUI::renderServotron(ipList);
	GUI::render(snake, items, pause, started);
/*	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
	//	SDL_RenderCopy(renderer, texture, NULL, &texture_rect); 

	for (auto & s : snake)
		drawRect(s, 0xFF00FF);*/

	//	SDL_UpdateWindowSurface(this->window);
	(void)snake;
	(void)items;
	(void)pause;
	(void)started;

	SDL_GL_SwapWindow(window);
}

void	SDL_gui::getClickedIp(std::string & ip) const
{
	ip = GUI::getLastClickedIp();
}

void	SDL_gui::close(EVENT event)
{
	SDL_DestroyWindow(this->window);
	SDL_Quit();
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
