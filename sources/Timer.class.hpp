#pragma once

#include <ctime>

class Timer
{
	public:
		Timer(void);
		~Timer(void) = default;
		Timer(Timer const &) = delete;
		Timer & operator=(Timer const &) = delete;

		bool
		frame(std::size_t number);

	private:
		std::size_t		_frame;
};
