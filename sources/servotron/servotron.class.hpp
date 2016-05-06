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

			ClientInfo & operator=(ClientInfo const & c) {
				strcpy(this->ip, c.ip);
				this->id = c.id;
				return (*this);
			}
		};

		int							_interval;
		std::deque< ClientInfo >	_onlineClients;
		std::thread					_eventThread;
		bool						_scanStop;
		STATE						_state;
		ClientInfo					_currentConnectedServer;
		int							_sendDataSocket;
		int							_receiveDataSocket;
		std::string					_localIP;

		void		eventThread(void);
		char		keyToChar(const KEY k) const;
		KEY			charToKey(const char c) const;
		void		createUdpSocket(int & s, const int port, bool bind_port) const;
		void		sendDataToFloor(char *data, std::size_t size);
		void		sendDisconnection(void);

		void		readData(void);
		void		sendData(char *d, std::size_t size);
		void		sendData(char *d, std::size_t size, struct sockaddr_in *co);

	public:
		Servotron();
		Servotron(const Servotron&);
		virtual ~Servotron(void);

		Servotron &	operator=(Servotron const & src);

		void	setScanInterval(const int millis);
		void	getConnectedClients(Clients & clients) const;
		void	getClientEvent(Client const & c, KEY & key) const;
		void	startServer(void) const;
		void	stopServer(void) const;

		int		getInterval(void) const;
		void	setInterval(int tmp);

		void	getState(STATE & s) const;
		void	sendEvent(KEY & k);

		void	connectServer(const ClientInfo c);
		void	disconnectServer(void);

		void	scanClientsOnFloor(void);
};

std::ostream &	operator<<(std::ostream & o, Servotron const & r);

extern "C" {
	Servotron	*craeteServotron(void);
	void		deleteServotron(Servotron *s);
}

#endif
