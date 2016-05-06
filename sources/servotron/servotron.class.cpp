#include "servotron.class.hpp"
#include <string>
#include <unistd.h>
#include "SFML/Network.hpp"

#include <sys/select.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

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

void		Servotron::sendPokeToE1(void)
{
	std::deque< std::string >	ipList;	
	std::string					localIP;
	struct sockaddr_in			connection;

	ipList = genE1IPList();
	localIP = sf::IpAddress::getLocalAddress().toString();
	connection.sin_family = AF_INET;
	connection.sin_port = SENDING_PORT;
	//	while (42)
	//	{
	//		_onlineClients.empty();
	//		if (this->_scanStop)
	//			break ;
	for (std::string ip : ipList) {
		if (!inet_aton(ip.c_str(), &connection.sin_addr))
			perror("inet_aton");
		//std::cout << "checked connection for ip: " << ip << std::endl;
		std::cout << "poke [" << ip  << "]" << std::endl;
		if (ip.compare(localIP) && sendto(this->_sendDataSocket,
					(const void *)"POKE",
					sizeof("POKE"),
					0,
					(struct sockaddr *)&connection,
					sizeof(connection)) < 0) {
			perror("sendto");
		} else {
			_onlineClients.push_back(ClientInfo{const_cast< char *>(ip.c_str()), getClientId(ip.c_str())});
			std::cout << "connected : " << ip << std::endl;
		}
	}
	std::cout << "poked !\n";
	//for (auto & c : _onlineClients)
	//	std::cout << c.ip << std::endl;
}

void		Servotron::eventThread(void)
{
	fd_set	read_set;
	fd_set	old_set;

	FD_ZERO(&old_set);
	FD_SET(this->_receiveDataSocket, &old_set);
	while (42) {
		read_set = old_set;
		if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
			perror("select");
		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_set)) {
				if (i == this->_receiveDataSocket) {
					//to read datas ....
				} else {
					//do nothing
				}
			}
	}
}

void		Servotron::createUdpSocket(int & ret, const int port) const
{
	struct sockaddr_in  connection;
	const int           yes = 1;

	srand((unsigned int)clock());
	if ((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		perror("sock"), exit(-1);
	bzero(&connection, sizeof(connection));
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
//	if (bind_port)
//	{
		if (setsockopt(ret, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
			perror("setsockopt");
		if (setsockopt(ret, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			perror("setsockopt");
		connection.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(ret, (struct sockaddr *)&connection, sizeof(connection)) == -1)
			perror("(fatal) bind"), exit(-1);
//	}
}

Servotron::Servotron(void) :
	_interval(1000),
	_eventThread(&Servotron::eventThread, this),
	_scanStop(false),
	_state(STATE::SERVER),
	_currentConnectedServer({NULL, 0})
{
	createUdpSocket(this->_sendDataSocket, SENDING_PORT);
	createUdpSocket(this->_receiveDataSocket, SERVER_PORT);
//	if (this->_sendingSocket.bind(SENDING_PORT) != sf::Socket::Done)
//		std::cout << "can't bind udp sending socket !\n";
//	if (this->_serverSocket.bind(SERVER_PORT) != sf::Socket::Done)
//		std::cout << "can't bind udp receiver socket !\n";
}

Servotron::Servotron(Servotron const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Servotron::~Servotron(void)
{
	_scanStop = true;
	_eventThread.join();
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
	struct sockaddr_in			connection;

	connection.sin_family = AF_INET;
	connection.sin_port = SENDING_PORT;
	if (!inet_aton(this->_currentConnectedServer.ip, &connection.sin_addr))
		perror("inet_aton");

	data[0] = (char)BYTECODE::KEYEVENT;
	data[1] = keyToChar(k);

	if (sendto(this->_sendDataSocket, data, sizeof(data), 0, (struct sockaddr *)&connection, sizeof(connection)) < 0)
		perror("sendto");
}

void		Servotron::connectServer(const ClientInfo c)
{
	(void)c;
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

int			Servotron::getInterval(void) const { return (this->_interval); }
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
