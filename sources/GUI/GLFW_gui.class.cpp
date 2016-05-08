/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFW_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/08 19:01:54 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFW_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"
#define SCREEN_OVERLAY(x) glBegin(GL_QUADS); x; \
    	glVertex2f(-1, -1); \
    	glVertex2f(1, -1); \
    	glVertex2f(1, 1); \
    	glVertex2f(-1, 1);

KEY GLFW_gui::pressedKey = KEY::NONE;
std::map<int, KEY > keyMap = {
	{GLFW_KEY_UP, KEY::UP},
	{GLFW_KEY_DOWN, KEY::DOWN},
	{GLFW_KEY_LEFT, KEY::LEFT},
	{GLFW_KEY_RIGHT, KEY::RIGHT},
	{GLFW_KEY_P, KEY::PAUSE},
	{GLFW_KEY_ESCAPE, KEY::ESCAPE},
	{GLFW_KEY_0, KEY::ONE},
	{GLFW_KEY_1, KEY::TWO},
	{GLFW_KEY_2, KEY::THREE},
	{GLFW_KEY_ENTER, KEY::ENTER},
};

static void error_callback(int error, const char* description)
{
	(void)error;
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (keyMap.count(key))
	{
		if (action == 1)
			GLFW_gui::pressedKey = keyMap[key];
		else if (action == 0)
			GLFW_gui::pressedKey = KEY::NONE;
	}
	(void)scancode;
	(void)mods;
	(void)window;
}

GLFW_gui::GLFW_gui(void)
{
	glfwSetErrorCallback(error_callback);
	this->win = NULL;
}

GLFW_gui::GLFW_gui(GLFW_gui const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

GLFW_gui::~GLFW_gui(void)
{
	std::cout << "Destructor of GLFW_gui called" << std::endl;
}


GLFW_gui &	GLFW_gui::operator=(GLFW_gui const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

bool	GLFW_gui::loadItemTextures(void)
{
	GLuint	texID = 0;
	for (auto & tx : textureMap)
	{
		texID = SOIL_load_OGL_texture (
		 		tx.second.c_str(),
		 		SOIL_LOAD_AUTO,
		 		SOIL_CREATE_NEW_ID,
		 		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		 		);

		if (texID == 0 || (int)texID == -1) {
			std::cout << "failed to load" << tx.second << std::endl;
			continue ;
		}

		this->texMap.insert(std::pair< TEXTURE, GLuint >(tx.first, texID));
	}
	return (true);
}

bool	GLFW_gui::open(std::size_t width, std::size_t height, std::string const & name)
{
	// Initialise GLFW
	if(!glfwInit())
		return false;
	init = true;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	this->width = width;
	this->height = height;
	this->squareSize.x = winSize.x / width;
	this->squareSize.y = winSize.y / height;
	if (!(this->win = glfwCreateWindow(winSize.x, winSize.y, name.c_str(), NULL, NULL))) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(this->win);
	glfwSetKeyCallback(this->win, key_callback);

	if (!loadItemTextures())
		return (false);
	return (true);
}

void	GLFW_gui::getEvent(KEY & key) const
{
	key = this->pressedKey;
}

#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)
void	GLFW_gui::drawRect(Point const & p, const unsigned int color) const
{
	float	bx1, bx2, by1, by2;

	if (p.x > this->width || p.y > this->height)
		return ;
	getCasesBounds(p, bx1, by1, bx2, by2);

//	std::cout << bx1 << " | "<< by1 << " | "<< bx2 << " | "<< by2 << "\n";
   	glBegin(GL_QUADS);
		glColor1u(color);
    	glVertex2f(-1 + bx1, 1 - by1);
    	glVertex2f(-1 + bx2, 1 - by1);
		glColor1u(0x9300FF);
    	glVertex2f(-1 + bx2, 1 - by2);
    	glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}

void	GLFW_gui::getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const {
	x1 = ((float)this->squareSize.x * p.x / winSize.x) * 2;
	y1 = ((float)this->squareSize.y * p.y / winSize.y) * 2;
	x2 = ((float)this->squareSize.x * (p.x + 1) / winSize.x) * 2;
	y2 = ((float)this->squareSize.y * (p.y + 1) / winSize.y) * 2;
}

void	GLFW_gui::drawItem(Item const & i) const
{
	float	bx1, bx2, by1, by2;

	if (i.coo.x > this->width || i.coo.y > this->height)
		return ;
	getCasesBounds(i.coo, bx1, by1, bx2, by2);

//	std::cout << bx1 << " | "<< by1 << " | "<< bx2 << " | "<< by2 << "\n";
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, this->texMap.at(itemToTexture(i.type)));
   	glBegin(GL_QUADS);
    	glTexCoord2f(0, 0); glVertex2f(-1 + bx1, 1 - by1);
    	glTexCoord2f(0, 1); glVertex2f(-1 + bx2, 1 - by1);
    	glTexCoord2f(1, 1); glVertex2f(-1 + bx2, 1 - by2);
    	glTexCoord2f(1, 0); glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}

void	GLFW_gui::drawPauseScreen(void) const
{

}

void	GLFW_gui::drawStartScreen(void) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	SCREEN_OVERLAY(glColor4f(1, 1, 1, 0.5F));

	glClear(GL_DEPTH_BUFFER_BIT);
}

TEXTURE & 	GLFW_gui::itemToTexture(Item::TYPE const & i) const
{
	return (itemTexMap[i]);
}

void	GLFW_gui::render(Points const & snake, Items const & items, bool pause, bool started) const
{
    float	ratio;

	ratio = this->winSize.x / (float)this->winSize.y;
	(void)ratio;
	glViewport(0, 0, this->winSize.x, this->winSize.y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, this->winSize.x, this->winSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (pause && started)
		drawPauseScreen();
	if (!started && !pause)
		drawStartScreen();

	for (auto & s : snake) {
		drawRect(s, 0xFF00FF);
	}

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	for (auto & i : items)
		drawItem(i);
	glDisable(GL_TEXTURE_2D);

   	glFlush();

	glfwSwapBuffers(this->win);
	glfwPollEvents();
	(void)pause;
	(void)started;
}

void	GLFW_gui::close(EVENT event)
{
	glfwDestroyWindow(this->win);
	glfwTerminate();
	(void)event;
}

std::ostream &	operator<<(std::ostream & o, GLFW_gui const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	GLFW_gui	*createGUI(void)
	{
		return new GLFW_gui();
	}

	void		deleteGUI(GLFW_gui *gui)
	{
		delete gui;
	}
}
