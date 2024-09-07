#pragma once
#include "UIO.h"
#include "IUValue.h"
#include <ostream>

namespace uio
{
	enum E_IndentMode { Increment, Decrement, None };

	class UIOHelper
	{
	public:
		static UIO_API bool tryGetNumber(const std::string& value, double& result, E_UType& type);

		static bool ichar_equals(char a, char b);

		static bool iequals(const std::string& a, const std::string& b);

		static void handleIndent(std::ostream& stream, bool endl, int& indentLevel, E_IndentMode mode = E_IndentMode::None);

		static std::ostream& doIndent(std::ostream& stream, bool indent, int indentLevel, unsigned short indentValue = 4);
	};
}
