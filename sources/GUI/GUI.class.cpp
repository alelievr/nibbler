/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GUI.class.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 23:24:05 by alelievr          #+#    #+#             */
/*   Updated: 2017/05/26 22:05:36 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GUI.class.hpp"

#define SCREEN_OVERLAY(x) glBegin(GL_QUADS); x; \
    	glVertex2f(-1, -1); \
    	glVertex2f(1, -1); \
    	glVertex2f(1, 1); \
    	glVertex2f(-1, 1);

std::deque< unsigned int > colorList = {
	0x00FFBF,
	0xBF00FF,
	0xFF8000,
	0xFF0040,
	0xF7FE2E,
	0x2E9AFE,
	0xFE2EF7,
	0x00FF00,
	0xBCA9F5,
	0xE1F5A9,
	0xF5A9A9,
	0x0101DF
};

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
	_lastIpList.push_back("lol");
	_lastIpList.clear();
	_pixelFont.loadFontFile("assets/fonts/slkscr.ttf");
}

GUI::GUI(GUI const & src)
{
	*this = src;
}

GUI::~GUI(void)
{
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

static void	effectToColor(Player const & player)
{
	static int		blink = 0;

	if (player.invincible && (blink / 40) % 2)
		glColor4f(1, 1, 1, 0.7);
	else if (player.speed)
		glColor3f(0, 1, 0);
	else if (player.dead && (blink / 60) % 2)
		glColor4f(0.3, 0.3, 0.3, 0.7);
	else
		glColor4f(0, 0, 0, 0);
	blink++;
}

#define glColor1u(x) glColor3ub((char)(x >> 16), (char)(x >> 8), (char)x)
#define CMAX(x) ((x >= 200) ? 200 : x)
#define RED(x) ((x >> 16) & 0xFF)
#define GREEN(x) ((x >> 8) & 0xFF)
#define BLUE(x) ((x >> 0) & 0xFF)
#define LIGHTEN(x, c) x = ((CMAX((RED(x) + c))) << 16) | ((CMAX((GREEN(x) + c))) << 8) | CMAX((BLUE(x) + c))
void		GUI::drawRect(Point const & p, unsigned int color, Player const & player, bool first) const
{
	float	bx1, bx2, by1, by2;

	if (p.x > _width || p.y > _height)
		return ;
	getCasesBounds(p, bx1, by1, bx2, by2);

	printf("%x\n", color);
	if (first)
		LIGHTEN(color, 0x10);
	printf("%x\n", color);
//	std::cout << bx1 << " | "<< by1 << " | "<< bx2 << " | "<< by2 << "\n";
   	glBegin(GL_QUADS);
		glColor1u(color);
    	glVertex2f(-1 + bx1, 1 - by1);
    	glVertex2f(-1 + bx2, 1 - by1);
//		glColor1u(0x9300FF); //93
    	glVertex2f(-1 + bx2, 1 - by2);
    	glVertex2f(-1 + bx1, 1 - by2);
   	glEnd();

	glBegin(GL_QUADS);
		effectToColor(player);
    	glVertex2f(-1 + bx1, 1 - by1);
    	glVertex2f(-1 + bx2, 1 - by1);
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

void		GUI::drawPauseScreen(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	SCREEN_OVERLAY(glColor4f(1, 1, 1, 0.5F));

	_pixelFont.setSize(41);
	_pixelFont.setPadding(-6);
	_pixelFont.drawText(" PRESS P TO UNPAUSE", 0, _winSize.y - 300);

	glClear(GL_DEPTH_BUFFER_BIT);
}

void		GUI::drawStartScreen(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	SCREEN_OVERLAY(glColor4f(1, 1, 1, 0.5F));

	_pixelFont.setSize(41);
	_pixelFont.setPadding(-8);
	_pixelFont.drawText(" PRESS ENTER TO START", 0, _winSize.y - 300);

	glClear(GL_DEPTH_BUFFER_BIT);
}

void		GUI::render(Players const & players, Items const & items, bool paused, bool started, bool dead)
{
    float	ratio;

	ratio = _winSize.x / (float)_winSize.y;
	(void)ratio;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 1000, _winSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (paused && started)
		drawPauseScreen();
	if (!started && !paused)
		drawStartScreen();

	std::cout << "start render players \n";
	for (auto & player : players) {
		std::cout << "get player end\n";
		if (player.second.snake.size() == 0)
			continue ;

		auto const endBlock = *(player.second.snake.end() - 1);
		std::cout << "success !\n";
		std::cout << endBlock << std::endl;
		for (auto & s : player.second.snake) {
			drawRect(s, colorList[player.first % colorList.size()], player.second, s == endBlock);
		}
	}
	std::cout << "end render players\n";

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	for (auto & i : items)
		drawItem(i);
	glDisable(GL_TEXTURE_2D);

	if (dead)
	{
		std::cout << "display dead screen" << std::endl;
//		glViewport(0, 0, 1000, _winSize.y);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR);

		SCREEN_OVERLAY(glColor4f(1, 1, 1, 0.5F));

		_pixelFont.setSize(48);
		_pixelFont.setPadding(-5);
		_pixelFont.drawText("   GAME OVER", 0, _winSize.y - 300);

		glClear(GL_DEPTH_BUFFER_BIT);
		_servoUI.render(_lastIpList);
	}

   	glFlush();
}

void	GUI::updateGridSize(Point const & gd)
{
	_width = gd.x;
	_height = gd.y;
	_squareSize.x = _winSize.x / _width;
	_squareSize.y = _winSize.y / _height;
}

void	GUI::renderServotron(std::deque< std::string > const & ipList)
{
	_lastIpList = ipList;
	_servoUI.render(ipList);
}

void	GUI::onMouseClick(Point const & p)
{
	return (ServotronUI::onMouseClick(p));
}

std::string	GUI::getLastClickedIp(void) const
{
	return (_servoUI.getLastClickedIp());
}

void	GUI::open(std::size_t w, std::size_t h, Point const & ws)
{
	_winSize = ws;
	_width = w;
	_height = h;
	_squareSize.x = _winSize.x / _width;
	_squareSize.y = _winSize.y / _height;

	_servoUI.setWinSize((const int)ws.x, (const int)ws.y);
	loadTextures();
}

#include <unistd.h>
void	GUI::close(EVENT e)
{
	if (e == EVENT::GAMEOVER)
	{
	}
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
