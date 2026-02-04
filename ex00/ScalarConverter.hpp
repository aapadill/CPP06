/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 06:29:10 by aapadill          #+#    #+#             */
/*   Updated: 2026/01/29 16:24:48 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

class ScalarConverter {
	private:
		ScalarConverter();
		ScalarConverter(const ScalarConverter &other);
		ScalarConverter &operator=(const ScalarConverter &other);
		~ScalarConverter();

		enum LiteralType {
			LIT_CHAR,
			LIT_INT,
			LIT_FLOAT,
			LIT_DOUBLE,
			LIT_INVALID
		};

		static bool isNan(double value);
		static bool isInf(double value);
		static bool isFinite(double value);
		static bool isPseudoFloat(const std::string &s);
		static bool isPseudoDouble(const std::string &s);
		static bool isCharLiteral(const std::string &s);
		static bool isNumeric(const std::string &s, bool allowDot);
		static bool isIntLiteral(const std::string &s);
		static bool isFloatLiteral(const std::string &s);
		static bool isDoubleLiteral(const std::string &s);
		static LiteralType detectType(const std::string &s);
		static bool parseToDouble(const std::string &s, double &out, bool &overflow);
		static bool parseCharLiteral(const std::string &s, char &out);
		static bool parseIntLiteral(const std::string &s, int &out);
		static bool parseFloatLiteral(const std::string &s, float &out);
		static bool parseDoubleLiteral(const std::string &s, double &out);
		static std::string formatFloating(double value, bool withSuffixF);
		static void printCharFrom(double value);
		static void printIntFrom(double value);
		static void printFloatFrom(double value);
		static void printDoubleFrom(double value);
		static void printAllFromValue(double value);
		static void printAllImpossible();

	public:
		static void convert(const std::string &literal);
};
