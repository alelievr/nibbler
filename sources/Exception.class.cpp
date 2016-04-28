/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:31:50 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:33:09 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.class.hpp"

Exception::Exception(std::string && what) :
	std::runtime_error(what)
{
}
