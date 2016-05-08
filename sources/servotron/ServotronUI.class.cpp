#include "ServotronUI.class.hpp"


ServotronUI::ServotronUI(Servotron *s) : _servo(s)
{
	(void)_servo;
	std::cout << "Default constructor of ServotronUI called" << std::endl;
}

ServotronUI::ServotronUI(ServotronUI const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
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
