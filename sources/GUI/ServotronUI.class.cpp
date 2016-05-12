#include "ServotronUI.class.hpp"
#include "servotron.class.hpp"
#include <sstream>
#include <sys/time.h>

ServotronUI::ServotronUI(void)
{
	std::cout << "Default constructor of ServotronUI called" << std::endl;
	_basicFont.loadFontFile("assets/fonts/SourceSansPro-Semibold.otf");
}

ServotronUI::ServotronUI(ServotronUI const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

void		ServotronUI::onClick(sf::Vector2i const & pos)
{
	(void)pos;
}

static const char * ipToMacNumber(const char *ip) {
	static std::string		macNumber;
	std::istringstream		s(ip);
	char					dot;
	int						byte;

	s >> byte >> dot >> byte;
	macNumber = "e" + std::to_string(byte - 10);
	s >> dot >> byte;
	macNumber += "r" + std::to_string(byte);
	s >> dot >> byte;
	macNumber += "p" + std::to_string(byte);
	return (macNumber.c_str());
}

void		ServotronUI::renderClientCase(const char *name, const char *ip, int & y)
{
	_basicFont.setSize(32);
	_basicFont.drawText(name, 20, y);
	_basicFont.setSize(20);
	_basicFont.drawText((std::string("(") + ip + std::string(")")).c_str(), 120, y + 24);
	y += 60;
}

void		ServotronUI::renderClientList(std::deque< std::string > const & ipList)
{
	int		y = 20;

	glColor4f(1, 1, 1, 0.8);
	renderClientCase("Local game", "localhost", y);
	for (auto const & ip : ipList)
		renderClientCase(ipToMacNumber(ip.c_str()), ip.c_str(), y);
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
	    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void		ServotronUI::render(std::deque< std::string > const & ipList)
{
	struct timeval	now, begin;
	static bool		init = 0;
	float			timeSpend;
//	float ratio;

	if (!init && ((init = true)))
		gettimeofday(&begin, NULL);
	gettimeofday(&now, NULL);
	_ipList = ipList;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	std::cout << _width << std::endl;
//	ratio = _width / (float)_height;
	glViewport(1000, 0, 400, _height);
//	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	timeSpend = timedifference_msec(begin, now);
//	glRotatef(timeSpend * 100, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-3.6f, -2.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(3.6f, -2.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 3.6f, 0.f);
	glEnd();
	glPopMatrix();

	renderClientList(ipList);
}

std::string	ServotronUI::onMouseClick(Point const & p)
{
	if (p.x == 0 && p.y == 0)
		return ("");
	else
		return ("127.0.0.1");
}

void		ServotronUI::setWinSize(const int w, const int h)
{
	_width = w;
	_height = h;
}

ServotronUI::~ServotronUI(void)
{
	std::cout << "Destructor of ServotronUI called" << std::endl;
}

ServotronUI &	ServotronUI::operator=(ServotronUI const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, ServotronUI const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
