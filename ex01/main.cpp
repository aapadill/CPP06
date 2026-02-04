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

#include "Data.hpp"
#include "Serializer.hpp"
#include <iostream>

int main()
{
	Data data;
	data.number = 42;
	data.text = "hello";

	std::cout << "data: " << &data << "\n";
	std::cout << "number: " << data.number << " text: " << data.text << "\n";

	uintptr_t raw = Serializer::serialize(&data);
	std::cout << "raw: " << raw << "\n";

	Data *ptr = Serializer::deserialize(raw);
	std::cout << "ptr: " << ptr << "\n";
	if (ptr == &data)
		std::cout << "OK\n";
	else
		std::cout << "FAIL\n";
	return 0;
}
