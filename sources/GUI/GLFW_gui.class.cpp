/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFW_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/02 16:14:37 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFW_gui.class.hpp"
#include <unistd.h>
#include <map>

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


bool	GLFW_gui::open(std::size_t width, std::size_t height, std::size_t mapSize, std::string && name)
{
	// Initialise GLFW
	if(!glfwInit()) {
		return false;
		init = true;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	this->width = width;
	this->height = height;
	this->mapSize = mapSize;
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

void	GLFW_gui::render(Points const & snake, Items const & items, bool pause) const
{
    float	ratio;

	ratio = this->width / (float)this->height;
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT);

/*	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();*/

	glfwSwapBuffers(this->win);
	glfwPollEvents();
	(void)pause;
	(void)items;
	(void)snake;
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
