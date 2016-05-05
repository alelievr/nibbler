#ifndef SERVOTRON_HPP
# define SERVOTRON_HPP
# include <iostream>
# include <string>
# include <deque>
# include <thread>
# include "IServotron.interface.hpp"

class		Servotron : IServotron
{
	private:
		struct	ClientInfo
		{
			const char		*ip;
			const Client	id;
		};

		int							_interval;
		std::deque< ClientInfo >	_onlineClients;
		std::thread					_scanThread;
		bool						_scanStop;
		sf::UdpSocket				_serverSocket;
		STATE						_state;

		void	scanPortThread(void);

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
		void	sendEvent(KEY & k) const;

		void	connectServer(const Client c);
		void	disconnectClient(void);
};

std::ostream &	operator<<(std::ostream & o, Servotron const & r);

extern "C" {
	Servotron	*craeteServotron(void);
	void		deleteServotron(Servotron *s);
}

#endif
