#ifndef SERVOTRON_HPP
# define SERVOTRON_HPP
# include <iostream>
# include <string>
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

			ClientInfo & operator=(ClientInfo const & c) {
				strcpy(this->ip, c.ip);
				this->id = c.id;
				pts = c.pts;
				return (*this);
			}
		};

		int							_interval;
		std::deque< ClientInfo >	_onlineClients;
		std::thread					_eventThread;
		bool						_threadStop;
		STATE						_state;
		ClientInfo					_currentConnectedServer;
		int							_sendDataSocket;
		int							_receiveDataSocket;
		std::string					_localIP;

		void		eventThread(void);
		void		createUdpSocket(int & s, const int port, bool bind_port) const;
		void		sendDataToFloor(char *data, std::size_t size);
		void		sendDisconnection(void);

		void		readData(void);
		void		sendData(char *d, std::size_t size);
		void		sendData(char *d, std::size_t size, struct sockaddr_in *co);
		void		sendData(char *d, std::size_t size, std::string const & ip);
		void		sendEvent(KEY & k, std::string const & ip);

		void		makeConnectedPackage(char *data, bool type);
		void		makeDisconnectedPackage(char *data);

	public:
		Servotron();
		Servotron(const Servotron&) = delete;
		virtual ~Servotron(void);

		Servotron &	operator=(Servotron const & src) = delete;

		void	getConnectedClients(Clients & clients) const;
		void	startServer(void) const;
		void	stopServer(void) const;

		int		getInterval(void) const;
		void	setInterval(int tmp);
		void	getOnlineIpList(std::deque< std::string > & clist) const;

		void	sendEventToClients(Points & pts, Client const c = -1);

		void	getState(STATE & s) const;

		void	connectToServer(std::string const & ip);
		void	disconnectServer(void);

		void	scanClientsOnFloor(void);

		void	sendDataToConnectedClients(char *data, size_t size, Client cid = -1);
		void	popSnakeBlock(Point const & p);
		void	addSnakeBlock(Point const & p);
		void	getPlayerInfo(Players & players) const;
};

std::ostream &	operator<<(std::ostream & o, Servotron const & r);

extern "C" {
	Servotron	*craeteServotron(void);
	void		deleteServotron(Servotron *s);
}

#endif
