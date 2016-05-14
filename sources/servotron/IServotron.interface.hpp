/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServotron.interface.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 19:23:45 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/14 19:50:04 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <deque>
# include <ISlave.interface.hpp>

# define MAX_HOSTNAME		256
# define SERVER_PORT		10042
# define SENDING_PORT		10041
# define PACKAGE_SIZE		4
# define IP_SIZE			sizeof("127.127.127.127")

typedef int						Client;
typedef std::deque< Client >	Clients;

enum class	STATE
{
	CLIENT,
	SERVER,
};

enum	NETWORK_BYTES
{
	CONNECTION_BYTE		= '\x78',
	DISCONNECTION_BYTE	= '\x79',
	POKE_BYTE			= '\x01',
	REPLY_BYTE			= '\x02',
	ADD_BLOCK_BYTE		= '\x8E',
	POP_BLOCK_BYTE		= '\x8F',
};

class	IServotron
{
	public:
		virtual void		getConnectedClients(Clients & clients) const = 0;
		virtual void		connectToServer(std::string const & ip) = 0;
		virtual void		getOnlineIpList(std::deque< std::string > & clist) const = 0;
		virtual void		disconnectServer(void) = 0;
		virtual void		getState(STATE & s) const = 0;
		virtual void		getPlayerInfo(Players & players) const = 0;
		virtual void		popSnakeBlock(Point const & p) = 0;
		virtual void		addSnakeBlock(Point const & p) = 0;
		virtual void		getServerInfos(Point & gridSize) const = 0;
};

typedef IServotron	*(*createServotronF)(std::size_t w, std::size_t h);
typedef void		(*deleteServotronF)(IServotron *);

