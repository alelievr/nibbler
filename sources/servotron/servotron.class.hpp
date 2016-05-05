#ifndef SERVOTRON_HPP
# define SERVOTRON_HPP
# include <iostream>
# include <string>
# include <deque>
# include <thread>
# include "IServotron.interface.hpp"
# include "SFML/Network.hpp"

class		Servotron : IServotron
{
	private:
		struct	ClientInfo
		{
			char		*ip;
			Client		id;

			ClientInfo & operator=(ClientInfo const & c) {
				this->ip = c.ip;
				this->id = c.id;
				return (*this);
			}
		};

		int							_interval;
		std::deque< ClientInfo >	_onlineClients;
		std::thread					_scanThread;
		bool						_scanStop;
		sf::UdpSocket				_serverSocket;
		sf::UdpSocket				_sendingSocket;
		STATE						_state;
		ClientInfo					_currentConnectedServer;

		void		scanPortThread(void);
		char		keyToChar(const KEY k) const;
		KEY			charToKey(const char c) const;

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
};

std::ostream &	operator<<(std::ostream & o, Servotron const & r);

extern "C" {
	Servotron	*craeteServotron(void);
	void		deleteServotron(Servotron *s);
}

#endif
