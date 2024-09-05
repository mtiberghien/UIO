#pragma once
#include "IUValue.h"
#include <map>
#include <string>
#include "UIO.h"


namespace uio
{
	enum E_IndentMode { Increment, Decrement, None };


	class JsonIOHelper
	{
	public:
		static bool ichar_equals(char a, char b);

		static bool iequals(const std::string& a, const std::string& b);

		static void handleIndent(std::ostream& stream, bool indent, int& indentLevel, E_IndentMode mode = E_IndentMode::None);

		static std::ostream& doIndent(std::ostream& stream, bool indent, int& indentLevel);

		static bool findFirstNonSpaceCharacter(std::istream& stream);


		static E_ValueMetaType readValueType(std::istream& stream);


		static bool readNextCharacter(std::istream& stream, char expectedChar);

		static bool readStringValue(std::istream& stream, std::string& value);

		static bool  isNumber(const std::string& value);

		static bool setNonStringPrimitiveValue(std::string& primitive, double& result, E_UType& type);

		static bool readNonStringPrimitive(std::istream& stream, double& result, E_UType& type);

		static UIO_API bool tryGetNumber(const std::string& value, double& result, E_UType& type);

	};

	
}


