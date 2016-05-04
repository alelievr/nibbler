/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServotron.interface.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 19:23:45 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/04 19:40:23 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <deque>
#include <ISlave.interface.hpp>

typedef int						Client;
typedef std::deque< Client >	Clients;

class	IServotron
{
	public:
		virtual void		setScanInterval(const int millis) = 0;
		virtual void		getConnectedClients(Clients & clients) const = 0;
		virtual void		getClientEvent(Client const & c, KEY & key) const = 0;
}
