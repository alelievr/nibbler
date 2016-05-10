#ifndef FREETYPE_HPP
# define FREETYPE_HPP
# include <iostream>
# include <string>
# include "ft2build.h"
# include <Opengl/gl.h>
# include FT_FREETYPE_H

class		Freetype
{
	private:
		FT_Library		_library;
		FT_Face			_face;
		FT_GlyphSlot	_slot;


	public:
		Freetype();
		Freetype(const Freetype &);
		virtual ~Freetype(void);

		Freetype &	operator=(Freetype const & src);

		bool	loadFontFile(const char *ftf);

		void	setCharSize(const long charWidth, const long charHeight, const unsigned int Hresolution, const unsigned int Vresolution);

		void	drawText(const char *txt, unsigned int x, unsigned int y, float sx = 0.002f, float sy = 0.002f);

		void	setFontTransform(FT_Matrix & matix, FT_Vector & pen);

		FT_Library	getLibrary(void) const;
		void	setLibrary(FT_Library tmp);
		
		FT_Face	getFace(void) const;
		void	setFace(FT_Face tmp);
		
		FT_GlyphSlot	getSlot(void) const;
		void	setSlot(FT_GlyphSlot tmp);
};

std::ostream &	operator<<(std::ostream & o, Freetype const & r);

#endif