#include "ServotronUI.class.hpp"
#include "servotron.class.hpp"
#include <sstream>
#include <sys/time.h>

ServotronUI	*ServotronUI::self = NULL;

ServotronUI::ServotronUI(void) : clickBoxSize(45)
{
	ServotronUI::self = this;
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
	_basicFont.drawText((std::string("(") + ip + std::string(")")).c_str(), 220, y + 10);
	y += clickBoxSize;
}

void		ServotronUI::renderClientList(std::deque< std::string > const & ipList)
{
	int		y = 20;

	glColor4f(1, 1, 1, 0.8);
	renderClientCase("Local game", "localhost", y);
	for (auto const & ip : ipList)
		renderClientCase(ipToMacNumber(ip.c_str()), ip.c_str(), y);
}

void		ServotronUI::render(std::deque< std::string > const & ipList)
{
	this->ipList = ipList;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glViewport(1000, 0, 400, _height);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(clock() / 5000, 0, 0, 1);
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

void		ServotronUI::onMouseClick(Point const & p)
{
	if (!ServotronUI::self)
		return ;
	int				pos;
	int				clickBox = ServotronUI::self->clickBoxSize;

	if (p.x >= 1000)
	{
		pos = (p.y - (20 + clickBox)) / clickBox;
		std::cout << "pos = " << pos << std::endl;
		if ((int)p.y >= 20 && (int)p.y < 20 + clickBox)
			ServotronUI::self->setClickedIp("127.0.0.1");
		else if ((int)ServotronUI::self->ipList.size() > pos)
			ServotronUI::self->setClickedIp(ServotronUI::self->ipList[pos]);
	}
}

std::string	ServotronUI::getLastClickedIp(void) const
{
	return _clickedIP;
}

void		ServotronUI::setClickedIp(std::string const & ip)
{
	_clickedIP = ip;
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
