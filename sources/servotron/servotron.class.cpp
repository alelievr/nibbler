#include "servotron.class.hpp"

servotron::servotron(void)
{
	std::cout << "Default constructor of servotron called" << std::endl;
	this->_interval = 1000;
}

servotron::servotron(servotron const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

servotron::~servotron(void)
{
	std::cout << "Destructor of servotron called" << std::endl;
}

void		servotron::setScanInterval(const int millis)
{
	this->_interval = millis;
}

void		servotron::getConnectedClients(Clients & clients) const
{
	(void)clients;
}

void		servotron::getClientEvent(Client const & c, KEY & key) const
{
	(void)c;
	(void)key;
}


servotron &	servotron::operator=(servotron const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_interval = src.getInterval();
	}
	return (*this);
}

int		servotron::getInterval(void) const { return (this->_interval); }
void		servotron::setInterval(int tmp) { this->_interval = tmp; }

std::ostream &	operator<<(std::ostream & o, servotron const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
