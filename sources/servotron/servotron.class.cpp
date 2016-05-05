#include "servotron.class.hpp"
#include "SFML/Network.hpp"
#include <string>
#include <unistd.h>

static std::deque< std::string >	genE1IPList(void) {
	std::deque< std::string >	list;

	for (int i = 1; i <= 23; i++)
		for (int j = 1; j <= 13; j++)
			list.push_back(std::string("10.11.") + std::to_string(i) + std::string(".") + std::to_string(j));
	return (list);
}

static Client	getClientId(const char *ip) {
	return (std::hash<std::string>()(ip));
}

void		Servotron::scanPortThread(void)
{
	sf::TcpSocket				socket;
	std::deque< std::string >	ipList;	

	ipList = genE1IPList();
	while (42)
	{
		_onlineClients.empty();
		if (this->_scanStop)
			break ;
		for (std::string ip : ipList) {
			if (socket.connect(ip, SERVER_PORT) == sf::Socket::Done)
			{
				_onlineClients.push_back(ClientInfo{ip.c_str(), getClientId(ip.c_str())});
				std::cout << "connected : " << ip << std::endl;
			} else {
				//std::cout << "nope !" << std::endl;
			}
		}
		usleep(this->_interval * 1000);
	}
}

Servotron::Servotron(void) : _scanStop(false), _scanThread(&Servotron::scanPortThread, this), _state(STATE::SERVER)
{
	std::cout << "Default constructor of Servotron called" << std::endl;
	this->_interval = 1000;
}

Servotron::Servotron(Servotron const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Servotron::~Servotron(void)
{
	_scanStop = true;
	_scanThread.join();
	std::cout << "Destructor of Servotron called" << std::endl;
}

void		Servotron::setScanInterval(const int millis)
{
	this->_interval = millis;
}

void		Servotron::getConnectedClients(Clients & clients) const
{
	(void)clients;
}

void		Servotron::getClientEvent(Client const & c, KEY & key) const
{
	(void)c;
	(void)key;
}

void		Servotron::startServer(void) const
{
	
}

void		Servotron::stopServer(void) const
{

}

void		Servotron::getState(STATE & s) const
{

}

void		Servotron::sendEvent(KEY & k) const
{

}

Servotron &	Servotron::operator=(Servotron const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_interval = src.getInterval();
	}
	return (*this);
}

int		Servotron::getInterval(void) const { return (this->_interval); }
void		Servotron::setInterval(int tmp) { this->_interval = tmp; }

std::ostream &	operator<<(std::ostream & o, Servotron const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	Servotron	*createServotron(void) {
		return new Servotron();
	}

	void		deleteServotron(Servotron *s) {
		delete s;
	}
}

int			main(void) {
	Servotron	s;

	usleep(1 * 1000 * 1000);
	return (0);
}
