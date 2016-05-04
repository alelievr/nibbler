#ifndef SERVOTRON_HPP
# define SERVOTRON_HPP
# include <iostream>
# include <string>

class		servotron
{
	private:
		int	_interval;


	public:
		servotron();
		servotron(const servotron&);
		virtual ~servotron(void);

		servotron &	operator=(servotron const & src);

		void	setScanInterval(const int millis);

		void	getConnectedClients(Clients & clients) const;

		void	getClientEvent(Client const & c, KEY & key) const;

		int	getInterval(void) const;
		void	setInterval(int tmp);
};

std::ostream &	operator<<(std::ostream & o, servotron const & r);

#endif
