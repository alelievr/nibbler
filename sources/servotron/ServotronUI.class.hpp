#ifndef SERVOTRONUI_HPP
# define SERVOTRONUI_HPP
# include <iostream>
# include <string>
# include "SFML/Graphics.hpp"

class		Servotron;

class		ServotronUI
{
	private:
		Servotron	*_servo;

	public:
		ServotronUI(void) = delete;
		ServotronUI(Servotron *s);
		ServotronUI(const ServotronUI&);
		virtual ~ServotronUI(void);

		ServotronUI &	operator=(ServotronUI const & src);
};

std::ostream &	operator<<(std::ostream & o, ServotronUI const & r);

#endif
