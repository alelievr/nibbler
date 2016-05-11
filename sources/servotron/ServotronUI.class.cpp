#include "ServotronUI.class.hpp"
#include "servotron.class.hpp"
#include <sstream>

static void error_callback(int error, const char* description)
{
	(void)error;
	fputs(description, stderr);
}

ServotronUI::ServotronUI(Servotron *s) : _servo(s)
{
	std::cout << "Default constructor of ServotronUI called" << std::endl;
	_basicFont.loadFontFile("assets/fonts/SourceSansPro-Semibold.otf");

//	_win.create(sf::VideoMode(800, 600), "SFML window");
//	_win.setFramerateLimit(60);
	if(!glfwInit())
		;
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	if (!(this->_win = glfwCreateWindow(400, 1000, "Host list", NULL, NULL))) {
		std::cout << "failed to create window !\n" << std::endl;
		glfwTerminate();
	}
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glfwMakeContextCurrent(_win);
	glfwSwapInterval(1);
}

ServotronUI::ServotronUI(ServotronUI const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

void		ServotronUI::onClick(sf::Vector2i const & pos)
{
	(void)pos;
	(void)_servo;
}

static const char * ipToMacNumber(const char *ip) {
	static std::string		macNumber;
	std::istringstream		s(ip);
	char					dot;
	int						byte;

	s >> byte >> dot >> byte;
	macNumber = "e" + std::to_string(byte);
	s >> dot >> byte;
	macNumber += "r" + std::to_string(byte);
	s >> dot >> byte;
	macNumber += "p" + std::to_string(byte);
	return (macNumber.c_str());
}

void		ServotronUI::renderClientCase(const char *name, const char *ip, int & y)
{
	_basicFont.setSize(48);
	_basicFont.drawText(name, 20, y);
	_basicFont.setSize(20);
	_basicFont.drawText((std::string("(") + ip + std::string(")")).c_str(), 280, y + 24);
	y += 60;
}

void		ServotronUI::renderClientList(void)
{
	int		y = 20;
	_servo->getOnlineIpList(_ipList);

	glColor4f(1, 1, 1, 0.8);
	renderClientCase("Local game", "localhost", y);
	for (auto const & ip : _ipList)
	{
		renderClientCase(ipToMacNumber(ip.c_str()), ip.c_str(), y);
	}
}

void		ServotronUI::render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	float ratio;
	int width, height;

	glfwGetFramebufferSize(_win, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-2.4f, -1.6f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(2.4f, -1.6f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 2.4f, 0.f);
	glEnd();
	glPopMatrix();

	renderClientList();

	glfwSwapBuffers(_win);
	glfwPollEvents();
}

ServotronUI::~ServotronUI(void)
{
//	glfwDestroyWindow(_win);
//	glfwTerminate();
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
