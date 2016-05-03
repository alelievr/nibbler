#ifndef GLFW_GUI_HPP
# define GLFW_GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include "GLFW/glfw3.h"
# include "ISlave.interface.hpp"

class		GLFW_gui : ISlave
{
	private:
		GLFWwindow						*win;
		bool							init;
		std::size_t						width;
		std::size_t						height;
		std::size_t						mapSize;
		Point							squareSize;
		std::map<Item::TYPE, GLuint>	texMap;

		void drawRect(Point const & p, const unsigned int color) const;
		void drawItem(Item const & i) const;
		void getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const;
		bool loadItemTextures();

	public:
		static int		pressedKey;

		GLFW_gui();
		GLFW_gui(const GLFW_gui&);
		virtual ~GLFW_gui(void);

		GLFW_gui &	operator=(GLFW_gui const & src);

		bool open(std::size_t width, std::size_t height, std::size_t mapSize, std::string && name);
		void getEvent(KEY & key) const;
		void render(Points const & snake, Items const & items, bool pause) const;
		void close(EVENT event);
};

std::ostream &	operator<<(std::ostream & o, GLFW_gui const & r);

extern "C" {
	GLFW_gui	*createGUI(void);
	void		deleteGUI(GLFW_gui *gui);
}

#endif
