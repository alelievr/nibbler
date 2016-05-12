#ifndef GLFW_GUI_HPP
# define GLFW_GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include <deque>
# include "GLFW/glfw3.h"
# include "ISlave.interface.hpp"
# include "constant.h"
# include "GUI.class.hpp"

class		GLFW_gui : public ISlave, public GUI
{
	private:
		GLFWwindow						*win;
		bool							init;
		const Point						winSize = {WIN_W, WIN_H};

		void		drawRect(Point const & p, const unsigned int color) const;
		void		drawItem(Item const & i) const;
		void		drawPauseScreen(void) const;
		void		drawStartScreen(void) const;
		void		getCasesBounds(Point const & p, float & x1, float & y1, float & x2, float & y2) const;
		TEXTURE &	itemToTexture(Item::TYPE const & i) const;

	public:
		static KEY		pressedKey;
		static Point	mouse;

		GLFW_gui();
		GLFW_gui(const GLFW_gui&);
		virtual	~GLFW_gui(void);

		GLFW_gui &	operator=(GLFW_gui const & src);

		bool	open(std::size_t width, std::size_t height, std::string const & name);
		void	getEvent(KEY & key) const;
		void	render(Points const & snake, Items const & items, bool pause, bool started, std::deque< std::string > const & ipList);
		void	getClickedIp(std::string & ip) const;
		void	close(EVENT event);
};

std::ostream &	operator<<(std::ostream & o, GLFW_gui const & r);

extern "C" {
	GLFW_gui	*createGUI(void);
	void		deleteGUI(GLFW_gui *gui);
}

#endif
