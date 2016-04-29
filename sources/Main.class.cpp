#include "Main.class.hpp"

#include <iostream>
Main::Main(int argc, char **argv) :
	_args(initArgs(argc, argv)),
	_width(getDim(1, 10, 100)), _height(getDim(2, 25, 250))
{
	(void)_width, (void)_height;
}

int
Main::run(void)
{
	// main loop
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
Main::getDim(std::size_t idx, std::size_t min, std::size_t max)
{
	std::size_t		n;
	std::size_t		pos(0);

	if (idx >= _args.size()
			or (n = std::stoul(_args[idx], &pos)) < min
			or n > max
			or pos != _args[idx].size())
		throw Exception(ERROR::BAD_ARGS);
	return n;
}

