/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Freetype.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 23:36:18 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/11 02:54:49 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Freetype.class.hpp"
#include <algorithm>

Freetype::Freetype(void)
{
}

Freetype::Freetype(Freetype const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Freetype::~Freetype(void)
{
	std::cout << "Destructor of Freetype called" << std::endl;
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
	FT_Set_Char_Size(_face, 0, 16 * 64, 400, 1000);
	FT_Set_Pixel_Sizes(_face, 0, 16);
	_slot = _face->glyph;
	std::cout << "inialized font !\n";
	return (true);
}

void		Freetype::setCharSize(const long charWidth, const long charHeight, const unsigned int Hresolution, const unsigned int Vresolution)
{
	(void)charWidth;
	(void)charHeight;
	(void)Hresolution;
	(void)Vresolution;
}

static void	my_draw_bitmap(FT_Bitmap *b, int _x, int _y, FT_GlyphSlot _slot)
{
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glPixelTransferi(GL_MAP_COLOR, GL_TRUE);

	GLubyte		*bytes = new GLubyte[b->width * b->rows * 4];
	glPixelTransferi(GL_UNPACK_ALIGNMENT, 4);
	glPixelTransferi(GL_INDEX_OFFSET, 1);

	for(unsigned y = 0; y < b->rows; y++){
		for(unsigned x = 0; x < b->width; x++){
			for(int i = 0; i < 4; i++){
				bytes[(x + y * b->width) * 4 + i] = _slot->bitmap.buffer[x + b->width * y];
			}
		}
	}

	GLuint		tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _slot->bitmap.width, _slot->bitmap.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, tex);
	glVertex3f(-0.4f, -0.6f, 0.f);
	glVertex3f(0.4f, -0.6f, 0.f);
	glVertex3f(0.f, 0.4f, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	(void)_y;
	(void)_x;
/*	GLubyte rasters[24] = {
		0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00, 
		0xc0, 0x00, 0xff, 0x00, 0xff, 0x00, 0xc0, 0x00, 
		0xc0, 0x00, 0xc0, 0x00, 0xff, 0xc0, 0xff, 0xc0};
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBitmap(10, 12, 0.0, 0.0, 0.0, 0.0, rasters);*/
//	glRasterPos2f(0, 0);
//	glBitmap(b->width, b->rows, 0.0, 0.0, 0.0, 0.0, b->buffer);
//	glFlush();
/*	(void)x;
	(void)y;
	(void)b;
//	(void)rasters;
//	std::cout << "x = " << x << " | y = " << y << std::endl;
//	glColor3f(1.0, 1.0, 1.0);
//	glBitmap(b->width, b->rows, -10, -10, 0, 0, b->buffer);
	glFlush();
	for (unsigned i = 0; i < b->rows; i++)
	{
		for (unsigned j = 0; j < b->width; j++)
		{
			printf("\033[48;5;%im ", b->buffer[i * b->rows + j]);
		}
		std::cout << "\033[0m" << std::endl;
	}*/
//	(void)x;
//	(void)y;
}

void		Freetype::drawText(const char *txt, unsigned int x, unsigned int y, float sx, float sy)
{
	int	error;

	txt--;
	while (*++txt) {
		FT_UInt  glyph_index;

		/* retrieve glyph index from character code */
		glyph_index = FT_Get_Char_Index( _face, *txt );

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph( _face, glyph_index, FT_LOAD_DEFAULT );
		if ( error )
			continue;  /* ignore errors */

		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph( _face->glyph, FT_RENDER_MODE_NORMAL );
		if ( error )
			continue;

		/* now, draw to our target surface */
		my_draw_bitmap( &_slot->bitmap,
				x + _slot->bitmap_left,
				y - _slot->bitmap_top, _slot);

		/* increment pen position */
		x += _slot->advance.x >> 6;
		y += _slot->advance.y >> 6; /* not useful for now */
		(void)sx;
		(void)sy;
	}
}

void		Freetype::setFontTransform(FT_Matrix & matrix, FT_Vector & pen)
{
	(void)matrix;
	(void)pen;
}


Freetype &	Freetype::operator=(Freetype const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_library = src.getLibrary();
		this->_face = src.getFace();
		this->_slot = src.getSlot();
	}
	return (*this);
}

FT_Library		Freetype::getLibrary(void) const { return (this->_library); }
void		Freetype::setLibrary(FT_Library tmp) { this->_library = tmp; }

FT_Face		Freetype::getFace(void) const { return (this->_face); }
void		Freetype::setFace(FT_Face tmp) { this->_face = tmp; }

FT_GlyphSlot		Freetype::getSlot(void) const { return (this->_slot); }
void		Freetype::setSlot(FT_GlyphSlot tmp) { this->_slot = tmp; }

std::ostream &	operator<<(std::ostream & o, Freetype const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
