/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:34:07 by aapadill          #+#    #+#             */
/*   Updated: 2026/01/29 16:34:07 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Base {
	public:
		virtual ~Base();
};

class A : public Base {
};

class B : public Base {
};

class C : public Base {
};

Base *generate(void);
void identify(Base *p);
void identify(Base &p);
