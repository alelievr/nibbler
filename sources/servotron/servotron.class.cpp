#include "servotron.class.hpp"
#include "SFML/Network.hpp"
#include <string>
#include <regex>

std::deque< std::string >	Servotron::genLocalIPList(void) {
	std::deque< std::string >	list;
	char						ipData[4];

	inet_pton(AF_INET, _localIP.c_str(), ipData);
	if (_cluster)
		for (int i = 1; i <= 23; i++)
			for (int j = 1; j <= 13; j++)
				list.push_back(std::string("10.1") + std::to_string(ipData[1] - 10) + "." + std::to_string(i) + std::string(".") + std::to_string(j));
	else
	{
		//TODO: implement a local ip latency scan
		list.push_back("192.168.1.1");
	}
	return (list);
}

static Client	getClientId(const char *ip) {
	return (std::hash<std::string>()(ip));
}

void		Servotron::sendDataToFloor(char *data, std::size_t size)
{
	static std::deque< std::string >	ipList;
	static bool							init = false;
	struct sockaddr_in					connection;

	if (!init && ((init = true)))
		ipList = genLocalIPList();
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
	data[0] = NETWORK_BYTES::CONNECTION_BYTE;
	data[1] = (type) ? NETWORK_BYTES::POKE_BYTE : NETWORK_BYTES::REPLY_BYTE;
	inet_pton(AF_INET, this->_localIP.c_str(), data + 2);
	data[6] = static_cast< char >(_width);
	data[7] = static_cast< char >(_height);
}

void		Servotron::makeDisconnectedPackage(char *data)
{
	data[0] = NETWORK_BYTES::DISCONNECTION_BYTE;
	data[1] = POKE_BYTE;
	inet_pton(AF_INET, this->_localIP.c_str(), data + 2);
}

void		Servotron::makeMovementPackage(char *data, Point const & p, NETWORK_BYTES const & n)
{
	data[0] = (char)n;
//	std::cout << "created package with network byte " << std::hex << (int)n << std::endl;
	data[1] = (char)p.x;
	data[2] = (char)p.y;
	inet_pton(AF_INET, this->_localIP.c_str(), data + 3);
	data[7] = (char)this->_state;
}

void		Servotron::scanClientsOnFloor(void)
{
	char						data[8];

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

void		Servotron::sendDataToConnectedClients(char *data, size_t size, Client cid)
{
	for (auto & c : _onlineClients)
		if (c.id != cid)
			sendData(data, size, std::string(c.ip));
}

void		Servotron::readData(void)
{
	struct		sockaddr_in	co;
	socklen_t				colen;
	char					buff[0xF0];
	char					str[INET_ADDRSTRLEN];

	if (recvfrom(this->_receiveDataSocket, buff, sizeof(buff), 0, (struct sockaddr *)&co, &colen) <= 0)
		perror("recvfrom");

	if (buff[0] == NETWORK_BYTES::CONNECTION_BYTE) {
		inet_ntop(AF_INET, buff + 2, str, INET_ADDRSTRLEN);
		ClientInfo	tmp;
		strcpy(tmp.ip, str);
		tmp.id = getClientId(tmp.ip);
		tmp.gridSize.x = buff[6];
		tmp.gridSize.y = buff[7];
		if (std::find_if(_onlineClients.begin(), _onlineClients.end(),
					[&](ClientInfo & c) { return (c.id == tmp.id); }
					) == _onlineClients.end()) {
			_onlineClients.push_back(tmp);
			std::cout << "connected : " << str << std::endl;
		}
		if (buff[1] == NETWORK_BYTES::POKE_BYTE) {
			char	data[8];
			makeConnectedPackage(data, false);
			this->sendData(data, sizeof(data), str);
		}
	}
	if (buff[0] == NETWORK_BYTES::DISCONNECTION_BYTE) {
		inet_ntop(AF_INET, buff + 2, str, INET_ADDRSTRLEN);
		Client		cid = getClientId(str);
		_onlineClients.erase(std::find_if(_onlineClients.begin(), _onlineClients.end(),
					[&](ClientInfo & c) { return (c.id == cid); }));
		std::cout << "disconnected : " << str << std::endl;
	}
	if (buff[0] == NETWORK_BYTES::ADD_BLOCK_BYTE || buff[0] == (char)NETWORK_BYTES::POP_BLOCK_BYTE) {
		if (buff[7] == (char)STATE::SERVER && _state == STATE::SERVER)
			return ;
		inet_ntop(AF_INET, buff + 3, str, INET_ADDRSTRLEN);
		Client		cid = getClientId(str);

		auto client = std::find_if(_onlineClients.begin(), _onlineClients.end(),
				[&]
				(ClientInfo & c) {
					return (c.id == cid);
				});
		if (client != _onlineClients.end())
		{
			if (this->_state == STATE::SERVER)
				sendDataToConnectedClients(buff, 7, cid);
			if (buff[0] == NETWORK_BYTES::ADD_BLOCK_BYTE) {
				client->pts.push_back(Point{static_cast< std::size_t >(buff[1]), static_cast< std::size_t>(buff[2])});
			} else {
				auto const & to_remove_point = std::find_if(client->pts.begin(), client->pts.end(),
							[buff]
							(Point const & p) {
								return (p.x == static_cast< std::size_t >(buff[1]) && p.y == static_cast< std::size_t >(buff[2]));
							});
				if (to_remove_point != client->pts.end())
					client->pts.erase(to_remove_point);
			}
		}
	}

//	std::for_each(buff, buff + 7, [](char c){std::cout << std::hex << "\\x"	<< static_cast< int >(c) << " "; });
//	std::cout << std::endl;

//	for (auto & c : _onlineClients)
//		std::cout << c.ip << std::endl;
}

void		Servotron::sendData(char *data, std::size_t size, struct sockaddr_in *co)
{
	if (sendto(this->_sendDataSocket, data, size, 0, (struct sockaddr *)co, sizeof(*co)) < 0)
		{;}
//		perror("sendto");
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
		if (select(FD_SETSIZE, &read_set, NULL, NULL, &timeout) < 0)
			perror("select");
		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_set)) {
				if (i == this->_receiveDataSocket) {
					readData();
					//to read datas ....
				} else {
				//	printf("reading on %i ...\n", i);
					char	buf[128];
					read(i, buf, 128);
				}
			}
	}
}

void		Servotron::createUdpSocket(int & ret, const int port, bool bind_port) const
{
	struct sockaddr_in  connection;
	const int           yes = 1;

	//TODO socket open failing when reserved by system ?
	srand((unsigned int)clock());
	if ((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		perror("sock"), exit(-1);
	bzero(&connection, sizeof(connection));
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
	if (setsockopt(ret, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
		perror("setsockopt");
	if (setsockopt(ret, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		perror("setsockopt");
	if (bind_port)
	{
		connection.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(ret, (struct sockaddr *)&connection, sizeof(connection)) == -1)
			perror("(fatal) bind"), exit(-1);
	}
}

Servotron::Servotron(std::size_t w, std::size_t h) :
	_threadStop(false),
	_state(STATE::SERVER),
	_currentConnectedServer({{0}, 0, Points{0}, Point{w, h}}),
	_cluster(true)
{
	_width = w;
	_height = h;
	this->_localIP = sf::IpAddress::getLocalAddress().toString();

	if (!std::regex_match(_localIP, std::regex("10.1[1-3].[0-9]{1,2}.[0-9]{1,2}")))
	{
		std::cout << "not in the cluster, your local network will be scaned with the mask 192.168.1.*\n";
		_cluster = false;
	}
	createUdpSocket(this->_sendDataSocket, SENDING_PORT, false);
	createUdpSocket(this->_receiveDataSocket, SERVER_PORT, true);

	_eventThread = std::thread(&Servotron::eventThread, this);
	scanClientsOnFloor();
}

Servotron::~Servotron(void)
{
	this->sendDisconnection();

	_threadStop = true;
	_eventThread.join();
}

void		Servotron::getConnectedClients(Clients & clients) const
{
	clients.clear();

	for (ClientInfo const & c : _onlineClients)
		clients.push_back(c.id);
}

void		Servotron::getPlayerInfo(Players & players) const
{
	for (auto & c : _onlineClients)
		if (players.count(c.id))
			players[c.id].snake = c.pts;
}

void		Servotron::getState(STATE & s) const
{
	s = this->_state;
}

void		Servotron::popSnakeBlock(Point const & p)
{
	char			data[8];

	makeMovementPackage(data, p, NETWORK_BYTES::POP_BLOCK_BYTE);
	if (_state == STATE::CLIENT)
		this->sendData(data, sizeof(data), _currentConnectedServer.ip);
	else
		this->sendDataToConnectedClients(data, sizeof(data));
}

void		Servotron::addSnakeBlock(Point const & p)
{
	char			data[8];

	makeMovementPackage(data, p, NETWORK_BYTES::ADD_BLOCK_BYTE);
	if (_state == STATE::CLIENT)
		this->sendData(data, sizeof(data), _currentConnectedServer.ip);
	else
		this->sendDataToConnectedClients(data, sizeof(data));
}

void		Servotron::getServerInfos(Point & gridSize) const
{
	gridSize = _currentConnectedServer.gridSize;
}

void		Servotron::connectToServer(std::string const & ip)
{
	if (!ip.compare(_currentConnectedServer.ip))
		return ;
	if (!ip.compare("127.0.0.1"))
		this->_currentConnectedServer = {{0}, 0, Points{0}, Point{_width, _height}};
	Client id = getClientId(ip.c_str());

	for (auto const & tmpc : _onlineClients)
		if (tmpc.id == id)
		{
			this->_currentConnectedServer = tmpc;
			this->_state = STATE::CLIENT;
			break ;
		}
}

void		Servotron::disconnectServer(void)
{
	this->_currentConnectedServer = {{0}, 0, Points{0}, Point{_width, _height}};
	this->_state = STATE::SERVER;
}

int			Servotron::getInterval(void) const { return (this->_interval); }
void		Servotron::setInterval(int tmp) { this->_interval = tmp; }
void		Servotron::getOnlineIpList(std::deque< std::string > & clist) const
{
	clist.clear();
	for (auto const & c : _onlineClients)
		clist.push_back(c.ip);
}

std::ostream &	operator<<(std::ostream & o, Servotron const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

extern "C" {
	Servotron	*createServotron(std::size_t w, std::size_t h) {
		return new Servotron(w, h);
	}

	void		deleteServotron(Servotron *s) {
		delete s;
	}
}

int			main(void) {
	Servotron	s(30, 30);

	while (42)
		;
//	usleep(1 * 1000 * 1000);
	return (0);
}
