/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFW_gui.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 22:09:48 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/14 19:40:20 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFW_gui.class.hpp"
#include <unistd.h>
#include "SOIL.h"

KEY		GLFW_gui::pressedKey = KEY::NONE;
Point	GLFW_gui::mouse = {0, 0};
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

static void	mouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
	std::cout << "clicked on " << button << " action = " << action << std::endl;
	if (button == 0 && action == 1)
		GUI::onMouseClick(GLFW_gui::mouse);
	(void)window;
	(void)action;
	(void)mods;
}

static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
	(void)window;
	GLFW_gui::mouse.x = (int)xpos;
	GLFW_gui::mouse.y = (int)ypos;
}

GLFW_gui::GLFW_gui(void)
{
	glfwSetErrorCallback(error_callback);
	this->win = NULL;
}

GLFW_gui::GLFW_gui(GLFW_gui const & src) :
	GUI()
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

GLFW_gui::~GLFW_gui(void)
{
}

GLFW_gui &	GLFW_gui::operator=(GLFW_gui const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
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

	if (!(this->win = glfwCreateWindow(winSize.x, winSize.y, name.c_str(), NULL, NULL))) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(this->win);
	glfwSetKeyCallback(this->win, key_callback);
	glfwSetCursorPosCallback(this->win, mouseMoveCallback);
	glfwSetMouseButtonCallback(this->win, mouseClickCallback);

	GUI::open(width, height, winSize);

	return (true);
}

void	GLFW_gui::getClickedIp(std::string & ip) const
{
	ip = GUI::getLastClickedIp();
}

void	GLFW_gui::getEvent(KEY & key) const
{
	key = this->pressedKey;
}

void	GLFW_gui::render(Players const & players, Items const & items, bool pause, bool started, std::deque< std::string > const & ipList)
{
	GUI::render(players, items, pause, started);
	GUI::renderServotron(ipList);
	glfwSwapBuffers(this->win);
	glfwPollEvents();
	(void)ipList;
}

void	GLFW_gui::updateGridSize(Point const & gd)
{
	GUI::updateGridSize(gd);
}

void	GLFW_gui::close(EVENT event)
{
	if (event == EVENT::GAMEOVER)
		while (42)
		{
			GUI::close(event);
			glfwSwapBuffers(this->win);
			glfwPollEvents();
			if (pressedKey ==  KEY::ENTER or pressedKey == KEY::ESCAPE)
				break ;
		}
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
