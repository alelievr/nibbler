#include "Main.class.hpp"

#include <iostream>
Main::Main(int argc, char **argv) :
	_args(initArgs(argc, argv)),
	_width(initWidth()), _height(initHeight())
{
}

int
Main::run(void)
{
	// main loop
	(void)_width, (void)_height;
	return 0;
}

std::vector<std::string>
Main::initArgs(int argc, char **argv)
{
	std::vector<std::string>	args(static_cast<std::size_t>(argc));

	for (std::size_t i(0), len(static_cast<std::size_t>(argc)) ; i < len ; ++i)
		args[i] = argv[i];
	return args;
}

std::size_t
Main::initWidth(void)
{
	constexpr int	width_min(10);
	constexpr int	width_max(100);
	int				width;
	std::size_t		pos(0);

	if (_args.size() < 3ul/*4*/
			or (width = std::stoi(_args[1], &pos)) < width_min
			or width > width_max
			or pos != _args[1].size())
		throw Exception(ERROR::BAD_ARGS);
	return static_cast<std::size_t>(width);
}
std::size_t
Main::initHeight(void)
{
	constexpr int	height_min(25);
	constexpr int	height_max(250);
	int				height;
	std::size_t		pos(0);

	if ((height = std::stoi(_args[2], &pos)) < height_min
			or height > height_max
			or pos != _args[2].size())
		throw Exception(ERROR::BAD_ARGS);
	return static_cast<std::size_t>(height);
}
