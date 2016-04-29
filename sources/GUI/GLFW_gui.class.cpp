/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFW_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/04/30 01:45:10 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFW_gui.class.hpp"
#include <unistd.h>
#include <map>

int GLFW_gui::pressedKey = 0;
std::map<int, char > keyMap = {
	{GLFW_KEY_UP, 0},
	{GLFW_KEY_DOWN, 1},
	{GLFW_KEY_LEFT, 2},
	{GLFW_KEY_RIGHT, 3},
	{GLFW_KEY_P, 4},
	{GLFW_KEY_ESCAPE, 5},
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
			GLFW_gui::pressedKey |= 1 << keyMap[key];
		else if (action == 0)
			GLFW_gui::pressedKey &= ~(1 << keyMap[key]);
	}
	std::cout << GLFW_gui::pressedKey << std::endl;
	(void)scancode;
	(void)mods;
	(void)window;
}

GLFW_gui::GLFW_gui(void)
{
	if (!init)
	{
		glfwSetErrorCallback(error_callback);
		if (!glfwInit())
			this->init = false;
		else
			this->init = true;
	}
	this->win = NULL;
	std::cout << "Default constructor of GLFW_gui called" << std::endl;
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


bool	GLFW_gui::open(std::size_t width, std::size_t height, std::string && name)
{
	if (!init)
		return (false);
	if (!(this->win = glfwCreateWindow(width, height, name.c_str(), NULL, NULL)))
		return (false);
	this->width = width;
	this->height = height;
	glfwMakeContextCurrent(this->win);
	glfwSwapInterval(1);
	glfwSetKeyCallback(this->win, key_callback);
	(void)width;
	(void)height;
	(void)name;
	return (true);
}

void	GLFW_gui::getEvent(KEY & key) const
{
	(void)key;
}

void	GLFW_gui::render(Points const & snake, Items const & items, bool pause) const
{
    float	ratio;

	ratio = this->width / (float)this->height;
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
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
	glEnd();
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
