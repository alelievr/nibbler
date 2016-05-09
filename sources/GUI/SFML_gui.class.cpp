/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SFML_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/09 19:34:31 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SFML_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"

int SFML_gui::pressedKey = 0;

/*static void error_callback(int error, const char* description)
{
	(void)error;
	fputs(description, stderr);
}

static void key_callback(SFMLwindow* window, int key, int scancode, int action, int mods)
{
	if (keyMap.count(key))
	{
		if (action == 1)
			SFML_gui::pressedKey |= 1 << keyMap[key].first;
		else if (action == 0)
			SFML_gui::pressedKey &= ~(1 << keyMap[key].first);
	}
	std::cout << SFML_gui::pressedKey << std::endl;
	(void)scancode;
	(void)mods;
	(void)window;
}*/

SFML_gui::SFML_gui(void)
{
//	glfwSetErrorCallback(error_callback);
}

SFML_gui::SFML_gui(SFML_gui const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

SFML_gui::~SFML_gui(void)
{
	std::cout << "Destructor of SFML_gui called" << std::endl;
}


SFML_gui &	SFML_gui::operator=(SFML_gui const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

bool	SFML_gui::loadItemTextures(void)
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

bool	SFML_gui::open(std::size_t width, std::size_t height, std::string const & name)
{
	this->window.create(sf::VideoMode(winSize.x, winSize.y), name);

	if (!this->window.isOpen())
		return (false);

	this->width = width;
	this->height = height;
	this->squareSize.x = winSize.x / width;
	this->squareSize.y = winSize.y / height;

	if (!loadItemTextures())
		return (false);
	return (true);
	(void)name;
}

void	SFML_gui::getEvent(KEY & key) const
{
	(void)key;
}

/*#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)
void	SFML_gui::drawRect(Point const & p, const unsigned int color) const
{
	SFML_Rect	r;

	if (p.x > this->width || p.y > this->height)
		return ;
	getCasesBounds(p, r);

	SFML_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
	SFML_RenderFillRect(this->renderer, &r);
	(void)color;
}

void	SFML_gui::drawItem(Item const & i) const
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

void	SFML_gui::render(Points const & snake, Items const & items, bool pause, bool started) const
{
	if (window.isOpen())
    {
        // on inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
   /*     sf::Event event;
        while (window.pollEvent(event))
        {
            // évènement "fermeture demandée" : on ferme la fenêtre
            if (event.type == sf::Event::Closed)
                window.close();
        }*/
    //    window.display();
    }

	(void)snake;
	(void)items;
	(void)pause;
	(void)started;
}

void	SFML_gui::close(EVENT event)
{
	(void)event;
}

std::ostream &	operator<<(std::ostream & o, SFML_gui const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	SFML_gui		*createGUI(void)
	{
		return new SFML_gui();
	}

	void		deleteGUI(SFML_gui *gui)
	{
		delete gui;
	}
}
