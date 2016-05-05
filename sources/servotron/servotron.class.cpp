#include "servotron.class.hpp"
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
	std::string					localIP;

	ipList = genE1IPList();
	localIP = sf::IpAddress::getLocalAddress().toString();
	while (42)
	{
		_onlineClients.empty();
		if (this->_scanStop)
			break ;
		for (std::string ip : ipList) {
			if (ip.compare(localIP) && socket.connect(ip, CONNECTION_PORT) == sf::Socket::Done)
			{
				_onlineClients.push_back(ClientInfo{const_cast< char *>(ip.c_str()), getClientId(ip.c_str())});
				std::cout << "connected : " << ip << std::endl;
			} else {
				//std::cout << "nope !" << std::endl;
			}
		}
		std::cout << "scanned !\n";
		usleep(this->_interval * 1000);
	}
}

Servotron::Servotron(void) :
	_interval(1000),
	_scanThread(&Servotron::scanPortThread, this),
	_scanStop(false),
	_state(STATE::SERVER),
	_currentConnectedServer({NULL, 0})
{
	sf::TcpListener listener;

	// lie l'écouteur à un port
	if (listener.listen(CONNECTION_PORT) != sf::Socket::Done)
		std::cout << "can't listen to connection port !\n";
	if (this->_sendingSocket.bind(SENDING_PORT) != sf::Socket::Done)
		std::cout << "can't bind udp sending socket !\n";
	if (this->_serverSocket.bind(SERVER_PORT) != sf::Socket::Done)
		std::cout << "can't bind udp receiver socket !\n";
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
	//lol nothing, it's UDP sockets !
}

void		Servotron::stopServer(void) const
{
	//nothing is running !
}

void		Servotron::getState(STATE & s) const
{
	s = this->_state;
}

char		Servotron::keyToChar(const KEY key) const
{
	char	ret = '0';

	for (const auto & k : KEY()) {
		if (k == key)
			return (ret);
		ret++;
	}
	return (0);
}

KEY			Servotron::charToKey(const char c) const
{
	char	ret = '0';

	for (const auto & k : KEY()) {
		if (ret == c)
			return (k);
		ret++;
	}
	return (KEY::NONE);
}

void		Servotron::sendEvent(KEY & k)
{
	char			data[PACKAGE_SIZE];
	sf::IpAddress	receiver(this->_currentConnectedServer.ip);
	data[0] = (char)BYTECODE::KEYEVENT;
	data[1] = keyToChar(k);
	if (this->_serverSocket.send((void *)data, sizeof(data), receiver, (unsigned short)SERVER_PORT) != sf::Socket::Done)
		std::cout << "failed to send UDP package\n";
	(void)k;
}

void		Servotron::connectServer(const ClientInfo c)
{
	this->_currentConnectedServer = c;
	sf::TcpListener listener;

	listener.listen(CONNECTION_PORT);
//	this->_serverSocket.bind(SERVER_PORT);
}

void		Servotron::disconnectServer(void)
{
	this->_currentConnectedServer = {NULL, 0};
//	this->_serverSocket.close();
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

	usleep(10 * 1000 * 1000);
	return (0);
}
