#ifndef SERVOTRON_HPP
# define SERVOTRON_HPP
# include <iostream>
# include <string>
# include <cstring>
# include <deque>
# include <thread>
# include "IServotron.interface.hpp"

# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <stdlib.h>
# include <unistd.h>

class		Servotron : IServotron
{
	private:
		struct	ClientInfo
		{
			char		ip[IP_SIZE + 1];
			Client		id;
			Points		pts;
			Point		gridSize;
			bool		started = true;
			bool		paused = false;
			bool		invincible = false;
			bool		speed = false;

			ClientInfo() = default;
			ClientInfo(char byte, Client i, Points p, Point grid) :
				id(i),
				pts(p),
				gridSize(grid)
			{ ip[0] = byte; }

			ClientInfo & operator=(ClientInfo const & c) {
				strcpy(this->ip, c.ip);
				this->id = c.id;
				pts = c.pts;
				gridSize = c.gridSize;
				return (*this);
			}
		};

		std::deque< ClientInfo >	_onlineClients;
		std::thread					_eventThread;
		bool						_threadStop;
		STATE						_state;
		ClientInfo					_currentConnectedServer;
		ClientInfo					_me;
		int							_sendDataSocket;
		int							_receiveDataSocket;
		std::string					_localIP;
		std::size_t					_width;
		std::size_t					_height;
		bool						_cluster;
		Items						_items;

		void		eventThread(void);
		void		createUdpSocket(int & s, const int port, bool bind_port) const;
		void		sendDataToFloor(char *data, std::size_t size);
		void		sendDisconnection(void);
		std::deque< std::string >		genLocalIPList(void);

		void		readData(void);
		void		sendData(char *d, std::size_t size) const;
		void		sendData(char *d, std::size_t size, struct sockaddr_in *co) const;
		void		sendData(char *d, std::size_t size, std::string const & ip) const;
		void		sendEvent(KEY & k, std::string const & ip);

		void		makeConnectedPackage(char *data, bool type) const;
		void		makeDisconnectedPackage(char *data) const;
		void		makeMovementPackage(char *data, Point const & p, NETWORK_BYTES const & n) const;
		void		makeItemPackage(char *data, Item const & i, NETWORK_BYTES const & n) const;
		void		makeSyncItemPackage(char *data) const;
		void		makeSyncClientPackage(char *data, CLIENT_BYTE const & n, int value) const;

	public:
		Servotron(std::size_t w, std::size_t h);
		Servotron(const Servotron&) = delete;
		virtual ~Servotron(void);

		Servotron &	operator=(Servotron const & src) = delete;

		void	getConnectedClients(Clients & clients) const;
		void	startServer(void) const;
		void	stopServer(void) const;

		void	getOnlineIpList(std::deque< std::string > & clist) const;

		void	getState(STATE & s) const;

		void	connectToServer(std::string const & ip);
		void	getServerInfos(Point & gridSize) const;
		void	disconnectServer(void);

		void	scanClientsOnFloor(void);

		void	sendDataToConnectedClients(char *data, size_t size, Client cid = -1) const;
		void	popSnakeBlock(Point const & p);
		void	addSnakeBlock(Point const & p);
		void	getPlayerInfo(Players & players) const;
		Client	getLocalId(void) const;

		void	getItems(Items & is) const;
		void	deleteItem(Item const & i);
		void	addItem(Item const & i);

		void	updateClientState(CLIENT_BYTE const & n, int value);
};

std::ostream &	operator<<(std::ostream & o, Servotron const & r);

extern "C" {
	Servotron	*craeteServotron(void);
	void		deleteServotron(Servotron *s);
}

#endif
