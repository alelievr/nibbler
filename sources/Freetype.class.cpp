#include "Freetype.class.hpp"


Freetype::Freetype(void)
{
	std::cout << "Default constructor of Freetype called" << std::endl;
	this->_library = ;
	this->_face = ;
	this->_slot = ;
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
	
}

void		Freetype::setCharSize(const long charWidth, const long charHeight, const unsigned int Hresolution, const unsigned int Vresolution)
{
	
}

void		Freetype::drawText(const char *txt, const unsigned int x, const unsigned int y)
{
	
}

void		Freetype::setFontTransform(FT_Matrix & matix, FT_Vector & pen)
{
	
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
