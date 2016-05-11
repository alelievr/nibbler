/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDL_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/11 18:17:43 by alelievr         ###   ########.fr       */
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

bool	SDL_gui::loadItemTextures(void)
{
/*	GLuint	foodTex = 0;

	foodTex = SOIL_load_OGL_texture (
		 	"./sprites/pizza.png",
		 	SOIL_LOAD_AUTO,
		 	SOIL_CREATE_NEW_ID,
		 	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		 	);

	if (foodTex == 0 || (int)foodTex == -1)
		return (false);

	this->texMap.insert(std::pair< Item::TYPE, GLuint >(Item::TYPE::FOOD, foodTex));*/
	return (true);
}

bool	SDL_gui::open(std::size_t width, std::size_t height, std::string const & name)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return false;

	this->width = width;
	this->height = height;
	this->squareSize.x = winSize.x / width;
	this->squareSize.y = winSize.y / height;

	this->window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->winSize.x, this->winSize.y, SDL_WINDOW_SHOWN);
	if (!this->window)
		return (false);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	if (!loadItemTextures())
		return (false);
	return (true);
}

void	SDL_gui::getEvent(KEY & key) const
{
	SDL_Event		e;

	if (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			key = KEY::ESCAPE;
		if (e.type == SDL_KEYDOWN)
		{
		}
	}
}

/*#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)*/
void	SDL_gui::drawRect(Point const & p, const unsigned int color) const
{
	SDL_Rect	r;

	if (p.x > this->width || p.y > this->height)
		return ;
	getCasesBounds(p, r);

	SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(this->renderer, &r);
	(void)color;
}

void	SDL_gui::getCasesBounds(Point const & p, SDL_Rect & r) const {
	r.x = p.x * this->squareSize.x;
	r.y = p.y * this->squareSize.y;
	r.w = this->squareSize.x;
	r.h = this->squareSize.y;
}

/*void	SDL_gui::drawItem(Item const & i) const
{
	float	bx1, bx2, by1, by2;

	if (i.coo.x > this->width || i.coo.y > this->height)
		return ;
	getCasesBounds(i.coo, bx1, by1, bx2, by2);

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, this->texMap.at(i.type));
   	glBegin(GL_QUADS);
    	glTexCoord2f(0, 0); glVertex2f(-1 + bx1, 1 - by1);
    	glTexCoord2f(0, 1); glVertex2f(-1 + bx2, 1 - by1);
    	glTexCoord2f(1, 1); glVertex2f(-1 + bx2, 1 - by2);
    	glTexCoord2f(1, 0); glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}*/

void	SDL_gui::render(Points const & snake, Items const & items, bool pause, bool started)
{
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
	//	SDL_RenderCopy(renderer, texture, NULL, &texture_rect); 

	for (auto & s : snake)
		drawRect(s, 0xFF00FF);

	//	SDL_UpdateWindowSurface(this->window);
	(void)snake;
	(void)items;
	(void)pause;
	(void)started;

	SDL_RenderPresent(renderer);
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
