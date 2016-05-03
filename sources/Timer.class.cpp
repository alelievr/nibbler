#include "Timer.class.hpp"

Timer::Timer(void) :
	_frame(0ul)
{
}


bool
Timer::frame(std::size_t n)
{
	static timespec const	pause_duration{0, 16666667l}; // 60 fps

	nanosleep(&pause_duration, nullptr);
	++_frame;
	if (_frame >= n)
	{
		_frame = 0ul;
		return true;
	}
	return false;
}
