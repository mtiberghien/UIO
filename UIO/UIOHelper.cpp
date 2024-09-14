#include "pch.h"
#include "UIOHelper.h"
#include "IUValue.h"
#include <algorithm>
#include "UValue.h"

namespace uio
{
	bool UIOHelper::tryGetNumber(const std::string& value, double& result, E_UType& type)
	{
		char* end = nullptr;
		type = E_UType::Undefined;
		double val = strtod(value.c_str(), &end);
		if (end != value.c_str() && *end == '\0' && val != HUGE_VAL)
		{
			result = val;
			short s = (short)val;
			if (s == val)
			{
				type = E_UType::Short;
			}
			else
			{
				int i = (int)val;
				if (i == val)
				{
					type = E_UType::Int;
				}
				else
				{
					type = E_UType::Double;
				}
			}

			return true;
		};
		return false;
	}

	bool UIOHelper::ichar_equals(char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) ==
			std::tolower(static_cast<unsigned char>(b));
	}

	bool UIOHelper::iequals(const std::string& a, const std::string& b)
	{
		return a.size() == b.size() &&
			std::equal(a.begin(), a.end(), b.begin(), ichar_equals);
	}

	void UIOHelper::handleIndent(std::ostream& stream, bool endl, int& indentLevel, E_IndentMode mode)
	{
		switch (mode)
		{
		case E_IndentMode::Increment: indentLevel++; break;
		case E_IndentMode::Decrement: indentLevel--; break;
		default:break;
		}
		if (endl)
		{
			stream << std::endl;
		}
	}

	std::ostream& UIOHelper::doIndent(std::ostream& stream, bool indent, int indentLevel, unsigned short indentValue)
	{
		stream << std::string(indent * indentLevel * indentValue, ' ');
		return stream;
	}

	std::string UIOHelper::readWordLowerCase(std::istream& stream)
	{
		std::ostringstream s;
		while (!stream.eof())
		{
			char c = stream.peek();
			if (!std::isspace(c))
			{
				c = stream.get();
				if (std::isalnum(c))
				{
					s << static_cast<char>(std::tolower(c));
				}
			}
			else
			{
				break;
			}
		}
		return s.str();
	}

	std::string UIOHelper::toCamelCase(const std::string& value)
	{
		if (!value.empty())
		{
			std::istringstream input{ value };
			std::ostringstream output;
			if (findFirstNonSpaceCharacter(input))
			{
				output << static_cast<unsigned char>(std::tolower(input.get()));
				output << readWordLowerCase(input);
				while (findFirstNonSpaceCharacter(input))
				{
					output << static_cast<unsigned char>(std::toupper(input.get()));
					output << readWordLowerCase(input);
				}
			}
			return output.str();
		}
		return value;
	}

	std::string UIOHelper::toPascalCase(const std::string& value)
	{
		std::string result = toCamelCase(value);
		if (!result.empty())
		{
			result[0] = (unsigned char)std::toupper(result[0]);
		}
		return result;
	}

	bool UIOHelper::findFirstNonSpaceCharacter(std::istream& stream)
	{
		while (!stream.eof())
		{
			unsigned char c = stream.peek();
			if (std::isspace(c))
			{
				stream.get();
			}
			else
			{
				return c != EOF;
			}
		}
		return false;
	}

	bool UIOHelper::readNextCharacter(std::istream& stream, unsigned char expectedChar)
	{
		while (!stream.eof() && stream.peek() != expectedChar)
		{
			stream.get();
		}
		if (stream.peek() == expectedChar)
		{
			stream.get();
			return true;
		}
		return false;
	}

	std::string UIOHelper::toLower(const std::string& value)
	{
		std::string result{ value };
		std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {return std::tolower(c); });
		return result;
	}

    void UIOHelper::setValue(UValue& uValue, const std::string& sValue, E_UType desiredType)
	{
		E_UType t;
		double d;
		if (UIOHelper::tryGetNumber(sValue, d, t))
		{
			if (desiredType != E_UType::Undefined)
			{
				t = desiredType;
			}
			switch (t)
			{
			case E_UType::Bool: uValue = d == (double)1; break;
			case E_UType::Short: uValue = (short)d; break;
			case E_UType::Int: uValue = (int)d; break;
			case E_UType::Float: uValue = (float)d; break;
			case E_UType::String: uValue = sValue; break;
			default: uValue = d; break;
			}
		}
		else if (UIOHelper::iequals(sValue, "null"))
		{
			uValue = nullptr;
		}
		else
		{
			bool b_true = UIOHelper::iequals(sValue, "true");
			if (b_true || UIOHelper::iequals(sValue, "false"))
			{
				uValue = b_true;
			}
			else
			{
				uValue = sValue;
			}
		}

	}
}