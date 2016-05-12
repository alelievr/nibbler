#ifndef SDL_GUI_HPP
# define SDL_GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include <deque>
# include "SDL.h"
# include "ISlave.interface.hpp"
# include "constant.h"
# include "GUI.class.hpp"

class		SDL_gui : public ISlave, public GUI
{
	private:
		SDL_Window								*window;
		SDL_GLContext							glContext;
		const Point								winSize = {WIN_W, WIN_H};

	public:
		static int		pressedKey;

		SDL_gui();
		SDL_gui(const SDL_gui&);
		virtual ~SDL_gui(void);

		SDL_gui &	operator=(SDL_gui const & src);

		bool open(std::size_t width, std::size_t height, std::string const & name);
		void getEvent(KEY & key) const;
		void render(Points const & snake, Items const & items, bool pause, bool started, std::deque< std::string > const & ipList);
		void	getClickedIp(std::string & ip) const;
		void close(EVENT event);
};

std::ostream &	operator<<(std::ostream & o, SDL_gui const & r);

extern "C" {
	SDL_gui		*createGUI(void);
	void		deleteGUI(SDL_gui *gui);
}

#endif
