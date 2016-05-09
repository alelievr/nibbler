#include "ServotronUI.class.hpp"

static void error_callback(int error, const char* description)
{
	(void)error;
	fputs(description, stderr);
}

ServotronUI::ServotronUI(Servotron *s) : _servo(s)
{
	std::cout << "Default constructor of ServotronUI called" << std::endl;

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

void		ServotronUI::renderClientList(void)
{
	//_servo->getOnlineIpList(_ipList);

//	for (auto const & ip : _ipList)
//		;
}

void		ServotronUI::render(void)
{
/*	if (_win.isOpen()) {
		_win.setActive();
		_win.display();
	}*/

	float ratio;
	int width, height;

	glfwGetFramebufferSize(_win, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
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
