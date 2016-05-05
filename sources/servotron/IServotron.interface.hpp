/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServotron.interface.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 19:23:45 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/05 18:00:34 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <deque>
# include <ISlave.interface.hpp>

# define MAX_HOSTNAME   256
# define SERVER_PORT	10042
# define IP_SIZE		sizeof("127.127.127.127")

typedef int						Client;
typedef std::deque< Client >	Clients;

enum class	STATE
{
	CLIENT,
	SERVER,
	NONE
}

class	IServotron
{
	public:
		virtual void		setScanInterval(const int millis) = 0;
		virtual void		getConnectedClients(Clients & clients) const = 0;
		virtual void		getClientEvent(Client const & c, KEY & key) const = 0;
		virtual void		sendEvent(KEY & k) const = 0;
		virtual void		startServer(void) const = 0;
		virtual void		stopServer(void) const = 0;
		virtual void		getState(STATE & s) const = 0;
};

typedef IServotron	*(*createServotronF)(void);
typedef void		(*deleteServotronF)(IServotron *);

