/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Freetype.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 23:36:18 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/17 12:09:46 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Freetype.class.hpp"
#include <algorithm>

Freetype::Freetype(void) : _padding(0)
{
}

Freetype::Freetype(Freetype const & src)
{
	*this = src;
}

Freetype::~Freetype(void)
{
}

bool		Freetype::loadFontFile(const char *ftf)
{
	if (FT_Init_FreeType(&_library)) {
		std::cout << "failed to initialize the lib !\n";
		return (false);
	}
	if (FT_New_Face(_library, ftf, 0, &_face)) {
		std::cout << "failed to initialize the font\n";
		return (false);
	}
	_size = 48;
	FT_Set_Char_Size(_face, 0, 48 * 64, 400, 1000);
	FT_Set_Pixel_Sizes(_face, 0, 48);
	_slot = _face->glyph;
	std::cout << "inialized font !\n";
	return (true);
}

void		Freetype::setSize(const int size)
{
	if (size <= 0)
		return ;
	_size = size;
	FT_Set_Char_Size(_face, 0, size * 64, 400, 1000);
	FT_Set_Pixel_Sizes(_face, 0, size);
}

#include <unistd.h>
static void	my_draw_bitmap(FT_Bitmap *b, int _x, int _y, FT_GlyphSlot _slot, int _size)
{
	GLubyte		*bytes = new GLubyte[b->width * b->rows * 4];

	for(unsigned y = 0; y < b->rows; y++) {
		for(unsigned x = 0; x < b->width; x++) {
			for(int i = 0; i < 4; i++) {
				bytes[(x + y * b->width) * 4 + i] = _slot->bitmap.buffer[x + b->width * y];
			}
		}
	}

	static GLuint		tex = 0;

	if (tex == 0)
		glGenTextures(1, &tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, b->width, b->rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 400, 1000, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0); glVertex2d(_x + -00, _y + -00 + _size);
	glTexCoord2d(1.0,0.0); glVertex2d(_x +  b->width, _y + -00 + _size);
	glTexCoord2d(1.0,1.0); glVertex2d(_x +  b->width, _y +  b->rows + _size);
	glTexCoord2d(0.0,1.0); glVertex2d(_x + -00, _y +  b->rows + _size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	(void)_y;
	(void)_x;
}

void		Freetype::drawText(const char *txt, unsigned int x, unsigned int y)
{
	int	error;

	while (*txt) {
		FT_UInt  glyph_index;

		glyph_index = FT_Get_Char_Index(_face, *txt);

		if ((error = FT_Load_Glyph(_face, glyph_index, FT_LOAD_DEFAULT)))
			continue;

		if ((error = FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL)))
			continue;

		my_draw_bitmap(&_slot->bitmap,
				x + _slot->bitmap_left,
				y - _slot->bitmap_top, _slot, _size);

		x += _slot->advance.x >> 6;
		y += _slot->advance.y >> 6;

		x += _padding;
		txt++;
	}
}

void		Freetype::setFontTransform(FT_Matrix & matrix, FT_Vector & pen)
{
	(void)matrix;
	(void)pen;
}


Freetype &	Freetype::operator=(Freetype const & src)
{

	if (this != &src) {
		this->_library = src.getLibrary();
		this->_face = src.getFace();
		this->_slot = src.getSlot();
	}
	return (*this);
}

void			Freetype::setPadding(int pad) { this->_padding = pad; }

FT_Library		Freetype::getLibrary(void) const { return (this->_library); }
void			Freetype::setLibrary(FT_Library tmp) { this->_library = tmp; }

FT_Face			Freetype::getFace(void) const { return (this->_face); }
void			Freetype::setFace(FT_Face tmp) { this->_face = tmp; }

FT_GlyphSlot	Freetype::getSlot(void) const { return (this->_slot); }
void			Freetype::setSlot(FT_GlyphSlot tmp) { this->_slot = tmp; }

std::ostream &	operator<<(std::ostream & o, Freetype const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
