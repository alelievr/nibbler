/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:02:05 by fdaudre-          #+#    #+#             */
/*   Updated: 2016/04/28 21:34:39 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Exception.class.hpp"
#include "ISlave.interface.hpp"

#include <vector>
#include <string>

class Main
{
	public:
		Main(int argc, char **argv);
		~Main(void) = default;
		Main(Main const &) = delete;
		Main & operator=(Main const &) = delete;

		virtual int
		run(void) = 0;

	protected:
		std::vector<std::string>
		initArgs(int args, char **argv);
		std::size_t
		getDim(std::size_t idx, std::size_t min, std::size_t max);

		std::vector<std::string>	_args;

		std::size_t	const			_width;
		std::size_t	const			_height;
};

