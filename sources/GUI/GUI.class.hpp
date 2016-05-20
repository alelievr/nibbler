#ifndef GUI_HPP
# define GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include <deque>
# include <OpenGL/gl.h>
# include "ISlave.interface.hpp"
# include "constant.h"
# include "SOIL.h"
# include "Freetype.class.hpp"
# include "ServotronUI.class.hpp"

class		GUI
{
	private:
		std::size_t					_width;
		std::size_t					_height;
		Point						_winSize;
		Point						_squareSize;
		std::map< TEXTURE, GLuint >	_texMap;
		Freetype					_pixelFont;
		ServotronUI					_servoUI;
		std::deque< std::string >	_lastIpList;
		void		getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const;

	public:
		GUI(void);
		GUI(const GUI &);
		virtual ~GUI(void);

		GUI &	operator=(GUI const & src);

		bool		loadTextures(void);
		void		drawRect(Point const & p, unsigned int const color, Player const & player) const;
		void		drawItem(Item const & i) const;
		void		drawPauseScreen(void);
		void		drawStartScreen(void);
		void		render(Players const & players, Items const & items, bool paused, bool stared);
		void		updateGridSize(Point const & gd);
		void		renderServotron(std::deque< std::string > const & ipList);
		void		open(std::size_t width, std::size_t height, Point const & winSize);
		void		close(EVENT e);
		TEXTURE &	itemToTexture(Item::TYPE const & i) const;
		std::string	getLastClickedIp(void) const;

		static void	onMouseClick(Point const & position);
};

std::ostream &	operator<<(std::ostream & o, GUI const & r);

#endif
