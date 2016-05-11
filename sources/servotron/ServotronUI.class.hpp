#ifndef SERVOTRONUI_HPP
# define SERVOTRONUI_HPP
# include <iostream>
# include <string>
# include <thread>
# include <deque>
# include "SFML/Graphics.hpp"
# include "GLFW/glfw3.h"
# include "Freetype.class.hpp"

class		Servotron;

class		ServotronUI
{
	private:
		Servotron					*_servo;
		GLFWwindow					*_win;
		std::deque< std::string >	_ipList;
		Freetype					_basicFont;

		void		onClick(sf::Vector2i const & pos);
		void		renderClientList(void);
		void		renderClientCase(const char *name, const char *ip, int & y);

	public:
		ServotronUI(void) = delete;
		ServotronUI(Servotron *s);
		ServotronUI(const ServotronUI &);
		virtual ~ServotronUI(void);

		void		render(void);

		ServotronUI &	operator=(ServotronUI const & src);
};

std::ostream &	operator<<(std::ostream & o, ServotronUI const & r);

#endif
