#include "Freetype.class.hpp"

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
	FT_Set_Pixel_Sizes(_face, 0, 48);
	_slot = _face->glyph;
	return (true);
}

void		Freetype::setCharSize(const long charWidth, const long charHeight, const unsigned int Hresolution, const unsigned int Vresolution)
{
	(void)charWidth;
	(void)charHeight;
	(void)Hresolution;
	(void)Vresolution;
}

void		Freetype::drawText(const char *txt, unsigned int x, unsigned int y, float sx, float sy)
{
	txt--;
	while (*++txt) {
		if (FT_Load_Char(_face, *txt, FT_LOAD_RENDER))
			continue ;

		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				_slot->bitmap.width,
				_slot->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				_slot->bitmap.buffer
				);

		float x2 = x + _slot->bitmap_left * sx;
		float y2 = -y - _slot->bitmap_top * sy;
		float w = _slot->bitmap.width * sx;
		float h = _slot->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (_slot->advance.x/64) * sx;
		y += (_slot->advance.y/64) * sy;

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
