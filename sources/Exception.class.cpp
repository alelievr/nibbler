/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:31:50 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:46:43 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.class.hpp"

Exception::Exception(std::string && what) :
	std::runtime_error(what)
{
}

Exception::Exception(ERROR code) :
	std::runtime_error(getString(code))
{
}


std::string
Exception::getString(ERROR code)
{
	switch (code)
	{
		case ERROR::BAD_ARGS:
			return "Invalid arguments.";
	}
//	return "";
}
