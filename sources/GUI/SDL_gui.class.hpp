#ifndef SDL_GUI_HPP
# define SDL_GUI_HPP
# include <iostream>
# include <string>
# include <map>
# include "SDL.h"
# include "ISlave.interface.hpp"
# include "constant.h"

class		SDL_gui : ISlave
{
	private:
		SDL_Window								*window;
		SDL_Renderer							*renderer;
		std::size_t								width;
		std::size_t								height;
		Point									squareSize;
		std::map< Item::TYPE, SDL_Texture* >	texMap;
		const Point								winSize = {WIN_W, WIN_H};

		void drawRect(Point const & p, const unsigned int color) const;
//		void drawItem(Item const & i) const;
		void getCasesBounds(Point const & p, SDL_Rect & r) const;
		bool loadItemTextures();

	public:
		static int		pressedKey;

		SDL_gui();
		SDL_gui(const SDL_gui&);
		virtual ~SDL_gui(void);

		SDL_gui &	operator=(SDL_gui const & src);

		bool open(std::size_t width, std::size_t height, std::string && name);
		void getEvent(KEY & key) const;
		void render(Points const & snake, Items const & items, bool pause) const;
		void close(EVENT event);
};

std::ostream &	operator<<(std::ostream & o, SDL_gui const & r);

extern "C" {
	SDL_gui		*createGUI(void);
	void		deleteGUI(SDL_gui *gui);
}

#endif
