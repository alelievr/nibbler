/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.class.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:26:16 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:44:56 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <string>

enum class ERROR
{
	BAD_ARGS,
};

class Exception : public std::runtime_error
{
	public:
		Exception(std::string && what);
		Exception(ERROR code);

	private:
		std::string
		getString(ERROR code);
};
