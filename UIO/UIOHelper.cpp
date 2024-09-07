#include "pch.h"
#include "UIOHelper.h"
#include "IUValue.h"

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
}