#include "servotron.class.hpp"
#include "SFML/Network.hpp"
#include <string>

static std::deque< std::string >	genE1IPList(std::string const & localIP) {
	std::deque< std::string >	list;
	char						ipData[4];

	inet_pton(AF_INET, localIP.c_str(), ipData);
	for (int i = 1; i <= 23; i++)
		for (int j = 1; j <= 13; j++)
			list.push_back(std::string("10.1") + std::to_string(ipData[1] - 10) + "." + std::to_string(i) + std::string(".") + std::to_string(j));
	return (list);
}

static Client	getClientId(const char *ip) {
	return (std::hash<std::string>()(ip));
}

void		Servotron::sendDataToFloor(char *data, std::size_t size)
{
	std::deque< std::string >	ipList;	
	struct sockaddr_in			connection;

	ipList = genE1IPList(this->_localIP);
	connection.sin_family = AF_INET;
	connection.sin_port = htons(SERVER_PORT);

	for (std::string ip : ipList) {
		if (!inet_aton(ip.c_str(), &connection.sin_addr))
			perror("inet_aton1");
//		std::cout << "poke sended to " << ip  << ":" << SERVER_PORT << std::endl;
		if (ip.compare(this->_localIP))
 		   sendData(data, size, &connection);
	}
}

void		Servotron::makeConnectedPackage(char *data, bool type)
{
	data[0] = 'I';
	data[1] = 'P';
	data[2] = (type) ? POKE_BYTE : REPLY_BYTE;
	inet_pton(AF_INET, this->_localIP.c_str(), data + 3);
}

void		Servotron::makeDisconnectedPackage(char *data)
{
	data[0] = 'P';
	data[1] = 'I';
	data[2] = POKE_BYTE;
	inet_pton(AF_INET, this->_localIP.c_str(), data + 3);
}

void		Servotron::scanClientsOnFloor(void)
{
	char						data[7];

	makeConnectedPackage(data, true);
	std::cout << "scaning for client on current floor" << std::endl;
	sendDataToFloor(data, sizeof(data));
}

void		Servotron::sendDisconnection(void)
{
	char						data[7];

	makeDisconnectedPackage(data);
	sendDataToFloor(data, sizeof(data));
}

void		Servotron::readData(void)
{
	struct		sockaddr_in	co;
	socklen_t				colen;
	char					buff[0xF0];
	char					str[INET_ADDRSTRLEN];

	if (recvfrom(this->_receiveDataSocket, buff, sizeof(buff), 0, (struct sockaddr *)&co, &colen) <= 0)
		perror("recvfrom");

	if (!strncmp(buff, "IP", 2)) {
		inet_ntop(AF_INET, buff + 3, str, INET_ADDRSTRLEN);
		ClientInfo	tmp;
		strcpy(tmp.ip, str);
		tmp.id = getClientId(tmp.ip);
		if (std::find_if(_onlineClients.begin(), _onlineClients.end(),
					[&](ClientInfo & c) { return (c.id == tmp.id); }
					) == _onlineClients.end()) {
			_onlineClients.push_back(tmp);
			std::cout << "connected : " << str << std::endl;
		}
		else
			std::cout << "already in list !\n";
		if (buff[2] == POKE_BYTE) {
			std::cout << "sending poke reply !\n";
			char	data[7];
			makeConnectedPackage(data, false);
			this->sendData(data, sizeof(data), str);
		}
	}
	if (!strncmp(buff, "PI", 2)) {
		inet_ntop(AF_INET, buff + 3, str, INET_ADDRSTRLEN);
		Client		cid = getClientId(str);

		//std::remove_if(_onlineClients.begin(), _onlineClients.end(), [&](ClientInfo c){ return (c.id == id); });
		_onlineClients.erase(std::find_if(_onlineClients.begin(), _onlineClients.end(),
					[&](ClientInfo & c) { return (c.id == cid); }));
		std::cout << "disconnected : " << str << std::endl;
	}
	if (buff[0] == (char)BYTECODE::KEYEVENT) {
		inet_ntop(AF_INET, buff + 2, str, INET_ADDRSTRLEN);
		Client		cid = getClientId(str);

		auto client = std::find_if(_onlineClients.begin(), _onlineClients.end(), [&](ClientInfo & c) { return (c.id == cid); });
		if (client != _onlineClients.end())
			client->lastEvent = charToKey(buff[1]);
	}

	std::cout << "debug [" << buff << "]" << std::endl;
	std::cout << "client list: " << std::endl;
	for (auto & c : _onlineClients)
		std::cout << c.ip << std::endl;
}

void		Servotron::sendData(char *data, std::size_t size, struct sockaddr_in *co)
{
	if (sendto(this->_sendDataSocket, data, size, 0, (struct sockaddr *)co, sizeof(*co)) < 0)
		perror("sendto");
}

void		Servotron::sendData(char *data, std::size_t size)
{
	struct sockaddr_in			connection;

	connection.sin_family = AF_INET;
	connection.sin_port = htons(SERVER_PORT);
	if (!inet_aton(this->_currentConnectedServer.ip, &connection.sin_addr)) {
		perror("inet_aton");
		return ;
	}

	this->sendData(data, size, &connection);
}

void		Servotron::sendData(char *data, std::size_t size, std::string const & ip)
{
	struct sockaddr_in			connection;

	connection.sin_family = AF_INET;
	connection.sin_port = htons(SERVER_PORT);
	if (!inet_aton(ip.c_str(), &connection.sin_addr)) {
		perror("inet_aton");
		return ;
	}

	this->sendData(data, size, &connection);
}

void		Servotron::eventThread(void)
{
	fd_set			read_set;
	fd_set			old_set;
	struct timeval	timeout;

	FD_ZERO(&old_set);
	FD_ZERO(&read_set);
	FD_SET(this->_receiveDataSocket, &old_set);
	FD_SET(this->_receiveDataSocket, &read_set);
	timeout.tv_sec = 0;
	timeout.tv_usec = 10 * 1000;
	while (42) {
		read_set = old_set;
		if (this->_threadStop)
			break ;
//		std::cout << "waiting for event on port " << SERVER_PORT << "\n";
		if (select(FD_SETSIZE, &read_set, NULL, NULL, &timeout) < 0)
			perror("select");
		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_set)) {
				if (i == this->_receiveDataSocket) {
					readData();
					//to read datas ....
				} else {
					printf("reading on %i ...\n", i);
					char	buf[128];
					read(i, buf, 128);
					//do nothing
				}
			}
	}
}

void		Servotron::createUdpSocket(int & ret, const int port, bool bind_port) const
{
	struct sockaddr_in  connection;
	const int           yes = 1;

	srand((unsigned int)clock());
	if ((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		perror("sock"), exit(-1);
	bzero(&connection, sizeof(connection));
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
	if (bind_port)
	{
		if (setsockopt(ret, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
			perror("setsockopt");
		if (setsockopt(ret, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			perror("setsockopt");
		connection.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(ret, (struct sockaddr *)&connection, sizeof(connection)) == -1)
			perror("(fatal) bind"), exit(-1);
	}
}

Servotron::Servotron(void) :
	_interval(1000),
	_threadStop(false),
	_state(STATE::SERVER),
	_currentConnectedServer({{0}, 0, KEY::NONE})
{
	std::cout << "constructed servotron !" << std::endl;
	this->_localIP = sf::IpAddress::getLocalAddress().toString();

	createUdpSocket(this->_sendDataSocket, SENDING_PORT, false);
	createUdpSocket(this->_receiveDataSocket, SERVER_PORT, true);

	_eventThread = std::thread(&Servotron::eventThread, this);
	scanClientsOnFloor();
}

Servotron::Servotron(Servotron const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Servotron::~Servotron(void)
{
	this->sendDisconnection();

	_threadStop = true;
	_eventThread.join();
	std::cout << "Destructor of Servotron called" << std::endl;
}

void		Servotron::setScanInterval(const int millis)
{
	this->_interval = millis;
}

void		Servotron::getConnectedClients(Clients & clients) const
{
	clients.clear();

	for (ClientInfo const & c : _onlineClients)
		clients.push_back(c.id);
}

void		Servotron::getClientEvent(Client const & cid, KEY & key) const
{
	key = KEY::NONE;

	const auto client = std::find_if(_onlineClients.begin(), _onlineClients.end(),
			[cid](ClientInfo const & cl) { return (cl.id == cid); });
	if (client != _onlineClients.end())
		key = client->lastEvent;
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
	char			data[6];

	data[0] = (char)BYTECODE::KEYEVENT;
	data[1] = keyToChar(k);
	inet_pton(AF_INET, this->_localIP.c_str(), data + 2);
	this->sendData(data, sizeof(data));
}

void		Servotron::connectServer(const ClientInfo c)
{
	this->_currentConnectedServer = c;
	(void)c;
}

void		Servotron::disconnectServer(void)
{
	this->_currentConnectedServer = {{0}, 0, KEY::NONE};
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

	while (42);
//	usleep(1 * 1000 * 1000);
	return (0);
}
