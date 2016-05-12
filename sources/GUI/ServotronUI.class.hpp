#ifndef SERVOTRONUI_HPP
# define SERVOTRONUI_HPP
# include <iostream>
# include <string>
# include <thread>
# include <deque>
# include "SFML/Graphics.hpp"
# include "GLFW/glfw3.h"
# include "Freetype.class.hpp"
# include "ISlave.interface.hpp"

class		Servotron;

class		ServotronUI
{
	private:
		Freetype					_basicFont;
		std::deque< std::string >	_ipList;
		int							_width;
		int							_height;

		void		onClick(sf::Vector2i const & pos);
		void		renderClientList(std::deque< std::string > const & ipList);
		void		renderClientCase(const char *name, const char *ip, int & y);

	public:
		ServotronUI(void);
		ServotronUI(const ServotronUI &);
		virtual ~ServotronUI(void);

		void		setWinSize(const int w, const int h);
		void		render(std::deque< std::string > const & ipList);
		std::string	onMouseClick(Point const & pos);

		ServotronUI &	operator=(ServotronUI const & src);
};

std::ostream &	operator<<(std::ostream & o, ServotronUI const & r);

#endif
