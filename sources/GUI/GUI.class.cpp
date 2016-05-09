/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GUI.class.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 23:24:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/09 02:06:35 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GUI.class.hpp"

#define SCREEN_OVERLAY(x) glBegin(GL_QUADS); x; \
    	glVertex2f(-1, -1); \
    	glVertex2f(1, -1); \
    	glVertex2f(1, 1); \
    	glVertex2f(-1, 1);

std::map< TEXTURE, std::string > textureMaps = {
	{TEXTURE::FOOD, "assets/sprites/pizza.png"},
	{TEXTURE::POOP, "assets/sprites/pizza.png"}, //TODO change
	{TEXTURE::WALL, "assets/sprites/pizza.png"}, //TODO change
	{TEXTURE::START, "assets/sprites/start.png"},
	{TEXTURE::GAMEOVER, "assets/sprites/gameover.png"},
};

std::map< Item::TYPE, TEXTURE > itemTexMaps = {
	{Item::TYPE::FOOD, TEXTURE::FOOD},
	{Item::TYPE::POOP, TEXTURE::POOP},
	{Item::TYPE::WALL, TEXTURE::WALL},
};

GUI::GUI(void)
{
	std::cout << "Default constructor of GUI called" << std::endl;
}

GUI::GUI(GUI const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

GUI::~GUI(void)
{
	std::cout << "Destructor of GUI called" << std::endl;
}

bool		GUI::loadTextures(void)
{
	GLuint	texID = 0;
	for (auto & tx : textureMaps)
	{
#ifndef	SFML
		texID = SOIL_load_OGL_texture (
		 		tx.second.c_str(),
		 		SOIL_LOAD_AUTO,
		 		SOIL_CREATE_NEW_ID,
		 		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		 		);
#endif

		if (texID == 0 || (int)texID == -1) {
			std::cout << "failed to load" << tx.second << std::endl;
			continue ;
		}

		_texMap.insert(std::pair< TEXTURE, GLuint >(tx.first, texID));
	}
	return (true);
}

void	GUI::getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const {
	x1 = ((float)_squareSize.x * p.x / _winSize.x) * 2;
	y1 = ((float)_squareSize.y * p.y / _winSize.y) * 2;
	x2 = ((float)_squareSize.x * (p.x + 1) / _winSize.x) * 2;
	y2 = ((float)_squareSize.y * (p.y + 1) / _winSize.y) * 2;
}

#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)
void		GUI::drawRect(Point const & p, unsigned int const color) const
{
	float	bx1, bx2, by1, by2;

	if (p.x > _width || p.y > _height)
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

TEXTURE & 	GUI::itemToTexture(Item::TYPE const & i) const
{
	return (itemTexMaps[i]);
}

void		GUI::drawItem(Item const & i) const
{
	float	bx1, bx2, by1, by2;

	if (i.coo.x > _width || i.coo.y > _height)
		return ;
	getCasesBounds(i.coo, bx1, by1, bx2, by2);

//	std::cout << bx1 << " | "<< by1 << " | "<< bx2 << " | "<< by2 << "\n";
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, _texMap.at(itemToTexture(i.type)));
   	glBegin(GL_QUADS);
    	glTexCoord2f(0, 0); glVertex2f(-1 + bx1, 1 - by1);
    	glTexCoord2f(0, 1); glVertex2f(-1 + bx2, 1 - by1);
    	glTexCoord2f(1, 1); glVertex2f(-1 + bx2, 1 - by2);
    	glTexCoord2f(1, 0); glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();
}

void		GUI::drawPauseScreen(void) const
{
	
}

void		GUI::drawStartScreen(void) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	SCREEN_OVERLAY(glColor4f(1, 1, 1, 0.5F));

	glClear(GL_DEPTH_BUFFER_BIT);
}

void		GUI::render(Points const & snake, Items const & items, bool paused, bool started) const
{
    float	ratio;

	ratio = _winSize.x / (float)_winSize.y;
	(void)ratio;
	glViewport(0, 0, _winSize.x, _winSize.y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, _winSize.x, _winSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (paused && started)
		drawPauseScreen();
	if (!started && !paused)
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
}

void	GUI::open(std::size_t w, std::size_t h, Point const & ws)
{
	_winSize = ws;
	_width = w;
	_height = h;
	_squareSize.x = _winSize.x / _width;
	_squareSize.y = _winSize.y / _height;

	loadTextures();
}

GUI &	GUI::operator=(GUI const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, GUI const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}