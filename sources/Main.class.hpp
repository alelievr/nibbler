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

class Main
{
	public:
		Main(int argc, char **argv);
		~Main(void) = default;
		Main(Main const &) = delete;
		Main & operator=(Main const &) = delete;

		int
		run(void);

	private:
		void
		arguments(int argc, char **argv);

		std::size_t		_width;
		std::size_t		_height;
};

