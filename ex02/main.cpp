/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:34:07 by aapadill          #+#    #+#             */
/*   Updated: 2026/01/29 16:34:07 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(0)));

	int i = 0;
	while (i < 5)
	{
		Base *ptr = generate();
		std::cout << "ptr: ";
		identify(ptr);
		std::cout << "ref: ";
		identify(*ptr);
		delete ptr;
		++i;
	}
	return 0;
}
