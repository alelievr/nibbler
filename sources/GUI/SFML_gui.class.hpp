#ifndef SFML_GUI_HPP
# define SFML_GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include "ISlave.interface.hpp"
# include "constant.h"
# include <SFML/Window.hpp>

class		SFML_gui : ISlave
{
	private:
		sf::Window								window;
		std::size_t								width;
		std::size_t								height;
		Point									squareSize;
		const Point								winSize = {WIN_W, WIN_H};

//		void drawRect(Point const & p, const unsigned int color) const;
//		void drawItem(Item const & i) const;
//		void getCasesBounds(Point const & p, SFML_Rect & r) const;
		bool loadItemTextures();

	public:
		static int		pressedKey;

		SFML_gui();
		SFML_gui(const SFML_gui&);
		virtual ~SFML_gui(void);

		SFML_gui &	operator=(SFML_gui const & src);

		bool open(std::size_t width, std::size_t height, std::string const & name);
		void getEvent(KEY & key) const;
		void render(Points const & snake, Items const & items, bool pause) const;
		void close(EVENT event);
};

std::ostream &	operator<<(std::ostream & o, SFML_gui const & r);

extern "C" {
	SFML_gui		*createGUI(void);
	void		deleteGUI(SFML_gui *gui);
}

#endif
