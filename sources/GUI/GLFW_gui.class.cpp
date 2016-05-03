/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFW_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/03 03:20:55 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFW_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"

int GLFW_gui::pressedKey = 0;
std::map<int, std::pair< char, KEY > > keyMap = {
	{GLFW_KEY_UP, {0, KEY::UP}},
	{GLFW_KEY_DOWN, {1, KEY::DOWN}},
	{GLFW_KEY_LEFT, {2, KEY::LEFT}},
	{GLFW_KEY_RIGHT, {3, KEY::RIGHT}},
	{GLFW_KEY_P, {4, KEY::PAUSE}},
	{GLFW_KEY_ESCAPE, {5, KEY::ESCAPE}},
	{GLFW_KEY_0, {6, KEY::ONE}},
	{GLFW_KEY_1, {7, KEY::TWO}},
	{GLFW_KEY_2, {8, KEY::THREE}},
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
			GLFW_gui::pressedKey |= 1 << keyMap[key].first;
		else if (action == 0)
			GLFW_gui::pressedKey &= ~(1 << keyMap[key].first);
	}
	std::cout << GLFW_gui::pressedKey << std::endl;
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
	GLuint	foodTex = -1;
//	GLuint	poopTex = -1;

/*	foodTex = SOIL_load_OGL_texture (
		 "sprites/pizza.png",
		 SOIL_LOAD_AUTO,
		 SOIL_CREATE_NEW_ID,
		 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
		 );*/

	if (foodTex == 0)
		return (false);

	this->texMap.insert(std::pair< Item::TYPE, GLuint >(Item::TYPE::FOOD, foodTex));
	return (true);
	(void)foodTex;
}

bool	GLFW_gui::open(std::size_t width, std::size_t height, std::size_t mapSize, std::string && name)
{
	// Initialise GLFW
	if(!glfwInit()) {
		return false;
		init = true;
	}

	if (!loadItemTextures())
		return (false);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	this->width = width;
	this->height = height;
	this->mapSize = mapSize;
	this->squareSize.x = width / mapSize;
	this->squareSize.y = height / mapSize;
	if (!(this->win = glfwCreateWindow(width, height, name.c_str(), NULL, NULL))) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(this->win);
	glfwSetKeyCallback(this->win, key_callback);
	return (true);
}

void	GLFW_gui::getEvent(KEY & key) const
{
	for (auto & m : keyMap)
		if (this->pressedKey & (1 << m.second.first))
			key = m.second.second;
	(void)key;
}

#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)
void	GLFW_gui::drawRect(Point const & p, const unsigned int color) const
{
	float	bx1, bx2, by1, by2;

	if (p.x > this->mapSize || p.y > this->mapSize)
		return ;
	getCasesBounds(p, bx1, by1, bx2, by2);

   	glBegin(GL_QUADS);
		glColor1u(color);
    	glVertex2f(-1 + bx1, 1 - by1);
		glColor1u(0xFF0000);
    	glVertex2f(-1 + bx2, 1 - by1);
		glColor1u(0x00FF00);
    	glVertex2f(-1 + bx2, 1 - by2);
		glColor1u(0x0000FF);
    	glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}

void	GLFW_gui::getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const {
	x1 = ((float)this->squareSize.x * p.x / this->width) * 2;
	y1 = ((float)this->squareSize.y * p.y / this->height) * 2;
	x2 = ((float)this->squareSize.x * (p.x + 1) / this->width) * 2;
	y2 = ((float)this->squareSize.y * (p.y + 1) / this->height) * 2;
}

void	GLFW_gui::drawItem(Item const & i) const
{
	float	bx1, bx2, by1, by2;

	if (i.coo.x > this->mapSize || i.coo.y > this->mapSize)
		return ;
	getCasesBounds(i.coo, bx1, by1, bx2, by2);

   	glBegin(GL_QUADS);
		glBindTexture(GL_TEXTURE_2D, this->texMap.at(i.type));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glVertex2f(-1 + bx1, 1 - by1);
    	glVertex2f(-1 + bx2, 1 - by1);
    	glVertex2f(-1 + bx2, 1 - by2);
    	glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}

void	GLFW_gui::render(Points const & snake, Items const & items, bool pause) const
{
    float	ratio;

	ratio = this->width / (float)this->height;
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, this->width, this->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   	glClear(GL_COLOR_BUFFER_BIT);

	for (auto & s : snake)
		drawRect(s, 0xFF00FF);

	for (auto & i : items)
		drawItem(i);

   	glFlush();

	glfwSwapBuffers(this->win);
	glfwPollEvents();
	(void)pause;
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
