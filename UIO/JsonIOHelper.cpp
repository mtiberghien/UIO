#include "pch.h"
#include "JsonIOHelper.h"
#include "UValue.h"

namespace json
{
	const std::map<std::string, char> g_escapes_read = { {"\\r", '\r'} , {"\\n", '\n'}, {"\\t", '\t' }, { "\\f", '\f' }, { "\\v", '\v' } };

	bool JsonIOHelper::tryGetNumber(const std::string& value, double& result, E_UType& type)
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

	bool JsonIOHelper::ichar_equals(char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) ==
			std::tolower(static_cast<unsigned char>(b));
	}

	bool JsonIOHelper::iequals(const std::string& a, const std::string& b)
	{
		return a.size() == b.size() &&
			std::equal(a.begin(), a.end(), b.begin(), ichar_equals);
	}

	void JsonIOHelper::handleIndent(std::ostream& stream, bool indent, int& indentLevel, E_IndentMode mode)
	{
		switch (mode)
		{
		case E_IndentMode::Increment: indentLevel++; break;
		case E_IndentMode::Decrement: indentLevel--; break;
		default:break;
		}
		if (indent)
		{
			stream << std::endl;
		}
	}

	std::ostream& JsonIOHelper::doIndent(std::ostream& stream, bool indent, int& indentLevel)
	{
		stream << std::string(indent * indentLevel * 4, ' ');
		return stream;
	}

	bool JsonIOHelper::findFirstNonSpaceCharacter(std::istream& stream)
	{
		while (!stream.eof())
		{
			char c = stream.peek();
			if (std::isspace(c))
			{
				stream.get();
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	E_ValueMetaType JsonIOHelper::readValueType(std::istream& stream)
	{
		if (findFirstNonSpaceCharacter(stream))
		{
			switch (stream.peek())
			{
			case '{': return E_ValueMetaType::Object;
			case '[': return E_ValueMetaType::Array;
			case '"': return E_ValueMetaType::StringPrimitive;
			default: return E_ValueMetaType::NonStringPrimitive;
			}
		}
		return E_ValueMetaType::Error;
	}


	bool JsonIOHelper::readNextCharacter(std::istream& stream, char expectedChar)
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

	bool JsonIOHelper::readStringValue(std::istream& stream, std::string& value)
	{
		std::ostringstream s;
		if (readNextCharacter(stream, '"'))
		{
			while (!stream.eof() && stream.peek() != '"')
			{
				char c = (char)stream.get();
				if (c == '\\')
				{
					std::ostringstream skey;
					skey << c << (char)stream.peek();
					std::string key = skey.str();
					if (g_escapes_read.find(key) != g_escapes_read.end())
					{
						stream.get();
						c = g_escapes_read.at(key);
					}
				}
				s << c;
			}
			if (readNextCharacter(stream, '"'))
			{
				value = s.str();
				return true;
			}
		}
		return false;
	}

	bool JsonIOHelper::isNumber(const std::string& value)
	{
		double d;
		E_UType t;
		return tryGetNumber(value, d, t);
	}

	bool JsonIOHelper::setNonStringPrimitiveValue(std::string& primitive, double& result, E_UType& type)
	{
		if (iequals(primitive, "null"))
		{
			type = E_UType::Null;
			return true;
		}
		if (iequals(primitive, "true") || iequals(primitive, "false"))
		{
			result = (double)(iequals(primitive, "true"));
			type = E_UType::Bool;
			return true;
		}
		else
		{
			return tryGetNumber(primitive, result, type);
		}

	}

	bool JsonIOHelper::readNonStringPrimitive(std::istream& stream, double& result, E_UType& type)
	{
		std::ostringstream s;
		char c;
		while (!stream.eof())
		{
			c = stream.peek();
			if (!std::isspace(c) && c != '}' && c != ']' && c != ',')
			{
				s << (char)stream.get();
			}
			else
			{
				break;
			}
		}
		std::string value = s.str();
		return !value.empty() && setNonStringPrimitiveValue(value, result, type);
	}
}

