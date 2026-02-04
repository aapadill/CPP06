/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 06:30:01 by aapadill          #+#    #+#             */
/*   Updated: 2026/01/29 16:25:15 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

bool ScalarConverter::isNan(double value)
{
	return value != value;
}

bool ScalarConverter::isInf(double value)
{
	return value == std::numeric_limits<double>::infinity()
		|| value == -std::numeric_limits<double>::infinity();
}

bool ScalarConverter::isFinite(double value)
{
	return !isNan(value) && !isInf(value);
}

bool ScalarConverter::isPseudoFloat(const std::string &s)
{
	return s == "nanf" || s == "+inff" || s == "-inff" || s == "inff";
}

bool ScalarConverter::isPseudoDouble(const std::string &s)
{
	return s == "nan" || s == "+inf" || s == "-inf" || s == "inf";
}

bool ScalarConverter::isCharLiteral(const std::string &s)
{
	if (s.size() == 3 && s[0] == '\'' && s[2] == '\'')
		return true;
	if (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])))
		return true;
	return false;
}

bool ScalarConverter::isNumeric(const std::string &s, bool allowDot)
{
	if (s.empty())
		return false;
	size_t i = 0;
	if (s[i] == '+' || s[i] == '-')
		++i;
	if (i >= s.size()) //for cases with only a sign and nothing else
		return false;
	bool hasDigit = false;
	bool hasDot = false;
	while (i < s.size())
	{
		unsigned char ch = static_cast<unsigned char>(s[i]);
		if (std::isdigit(ch))
		{
			hasDigit = true;
			++i;
			continue;
		}
		if (allowDot && s[i] == '.' && !hasDot)
		{
			hasDot = true;
			++i;
			continue;
		}
		return false;
	}
	return hasDigit;
}

bool ScalarConverter::isIntLiteral(const std::string &s)
{
	return isNumeric(s, false);
}

bool ScalarConverter::isFloatLiteral(const std::string &s)
{
	if (s.size() < 2 || s[s.size() - 1] != 'f')
		return false;
	return isNumeric(s.substr(0, s.size() - 1), true);
}

bool ScalarConverter::isDoubleLiteral(const std::string &s)
{
	if (s.find('.') == std::string::npos)
		return false;
	return isNumeric(s, true);
}

ScalarConverter::LiteralType ScalarConverter::detectType(const std::string &s)
{
	if (s.empty())
		return LIT_INVALID;
	if (isPseudoFloat(s))
		return LIT_FLOAT;
	if (isPseudoDouble(s))
		return LIT_DOUBLE;
	if (isCharLiteral(s))
		return LIT_CHAR;
	if (isFloatLiteral(s))
		return LIT_FLOAT;
	if (isDoubleLiteral(s))
		return LIT_DOUBLE;
	if (isIntLiteral(s))
		return LIT_INT;
	return LIT_INVALID;
}

bool ScalarConverter::parseToDouble(const std::string &s, double &out, bool &overflow)
{
	errno = 0;
	char *end = 0;
	out = std::strtod(s.c_str(), &end);
	if (end == s.c_str() || *end != '\0')
		return false;
	overflow = (errno == ERANGE);
	return true;
}

bool ScalarConverter::parseCharLiteral(const std::string &s, char &out)
{
	if (s.size() == 3 && s[0] == '\'' && s[2] == '\'')
	{
		out = s[1];
		return true;
	}
	if (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])))
	{
		out = s[0];
		return true;
	}
	return false;
}

bool ScalarConverter::parseIntLiteral(const std::string &s, int &out)
{
	errno = 0;
	char *end = 0;
	long val = std::strtol(s.c_str(), &end, 10);
	if (end == s.c_str() || *end != '\0')
		return false;
	if (errno == ERANGE)
		return false;
	if (val < std::numeric_limits<int>::min()
		|| val > std::numeric_limits<int>::max())
		return false;
	out = static_cast<int>(val);
	return true;
}

bool ScalarConverter::parseFloatLiteral(const std::string &s, float &out)
{
	std::string core = s.substr(0, s.size() - 1);
	double val = 0.0;
	bool overflow = false;
	if (!parseToDouble(core, val, overflow) || overflow)
		return false;
	if (isNan(val) || isInf(val))
	{
		out = static_cast<float>(val);
		return true;
	}
	if (val > std::numeric_limits<float>::max()
		|| val < -std::numeric_limits<float>::max())
		return false;
	out = static_cast<float>(val);
	return true;
}

bool ScalarConverter::parseDoubleLiteral(const std::string &s, double &out)
{
	double val = 0.0;
	bool overflow = false;
	if (!parseToDouble(s, val, overflow) || overflow)
		return false;
	out = val;
	return true;
}

std::string ScalarConverter::formatFloating(double value, bool withSuffixF)
{
	if (isNan(value))
	{
		if (withSuffixF)
			return "nanf";
		return "nan";
	}
	if (isInf(value))
	{
		if (value < 0)
		{
			if (withSuffixF)
				return "-inff";
			return "-inf";
		}
		if (withSuffixF)
			return "+inff";
		return "+inf";
	}
	std::ostringstream oss;
	double intpart = 0.0;
	if (std::modf(value, &intpart) == 0.0)
	{
		oss.setf(std::ios::fixed);
		oss << std::setprecision(1) << value;
	}
	else
	{
		int precision = 15;
		if (withSuffixF)
			precision = 7;
		oss << std::setprecision(precision) << value;
	}
	if (withSuffixF)
		oss << "f";
	return oss.str();
}

void ScalarConverter::printCharFrom(double value)
{
	std::cout << "char: ";
	if (!isFinite(value) || value < 0.0 || value > 127.0)
	{
		std::cout << "impossible\n";
		return;
	}
	char c = static_cast<char>(value);
	if (std::isprint(static_cast<unsigned char>(c)))
		std::cout << "'" << c << "'\n";
	else
		std::cout << "Non displayable\n";
}

void ScalarConverter::printIntFrom(double value)
{
	std::cout << "int: ";
	if (!isFinite(value)
		|| value < std::numeric_limits<int>::min()
		|| value > std::numeric_limits<int>::max())
	{
		std::cout << "impossible\n";
		return;
	}
	std::cout << static_cast<int>(value) << "\n";
}

void ScalarConverter::printFloatFrom(double value)
{
	std::cout << "float: ";
	if (isNan(value) || isInf(value))
	{
		std::cout << formatFloating(value, true) << "\n";
		return;
	}
	if (value > std::numeric_limits<float>::max()
		|| value < -std::numeric_limits<float>::max())
	{
		std::cout << "impossible\n";
		return;
	}
	float f = static_cast<float>(value);
	std::cout << formatFloating(static_cast<double>(f), true) << "\n";
}

void ScalarConverter::printDoubleFrom(double value)
{
	std::cout << "double: ";
	std::cout << formatFloating(value, false) << "\n";
}

void ScalarConverter::printAllFromValue(double value)
{
	printCharFrom(value);
	printIntFrom(value);
	printFloatFrom(value);
	printDoubleFrom(value);
}

void ScalarConverter::printAllImpossible()
{
	std::cout << "char: impossible\n";
	std::cout << "int: impossible\n";
	std::cout << "float: impossible\n";
	std::cout << "double: impossible\n";
}

ScalarConverter::ScalarConverter()
{
}

ScalarConverter::ScalarConverter(const ScalarConverter &other)
{
	(void)other;
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
	(void)other;
	return *this;
}

ScalarConverter::~ScalarConverter()
{
}

void ScalarConverter::convert(const std::string &literal)
{
	LiteralType type = detectType(literal);
	if (type == LIT_INVALID)
	{
		printAllImpossible();
		return;
	}

	if (type == LIT_CHAR)
	{
		char value = 0;
		if (!parseCharLiteral(literal, value))
		{
			printAllImpossible();
			return;
		}
		printAllFromValue(static_cast<double>(value));
		return;
	}

	if (type == LIT_INT)
	{
		int value = 0;
		if (!parseIntLiteral(literal, value))
		{
			double d = 0.0;
			if (!parseDoubleLiteral(literal, d))
			{
				printAllImpossible();
				return;
			}
			printAllFromValue(d);
			return;
		}
		printAllFromValue(static_cast<double>(value));
		return;
	}

	if (type == LIT_FLOAT)
	{
		if (isPseudoFloat(literal))
		{
			float value = 0.0f;
			if (literal == "nanf")
				value = std::numeric_limits<float>::quiet_NaN();
			else if (literal[0] == '-')
				value = -std::numeric_limits<float>::infinity();
			else
				value = std::numeric_limits<float>::infinity();
			printAllFromValue(static_cast<double>(value));
			return;
		}
		float value = 0.0f;
		if (!parseFloatLiteral(literal, value))
		{
			double d = 0.0;
			std::string core = literal.substr(0, literal.size() - 1);
			if (!parseDoubleLiteral(core, d))
			{
				printAllImpossible();
				return;
			}
			printAllFromValue(d);
			return;
		}
		printAllFromValue(static_cast<double>(value));
		return;
	}

	if (type == LIT_DOUBLE)
	{
		if (isPseudoDouble(literal))
		{
			double value = 0.0;
			if (literal == "nan")
				value = std::numeric_limits<double>::quiet_NaN();
			else if (literal[0] == '-')
				value = -std::numeric_limits<double>::infinity();
			else
				value = std::numeric_limits<double>::infinity();
			printAllFromValue(value);
			return;
		}
		double value = 0.0;
		if (!parseDoubleLiteral(literal, value))
		{
			printAllImpossible();
			return;
		}
		printAllFromValue(value);
		return;
	}
}
