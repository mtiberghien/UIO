#include "pch.h"
#include "JsonIOHelper.h"
#include "UIOHelper.h"
#include "UValue.h"


namespace uio
{
	const std::map<std::string, unsigned char> g_escapes_read = { {"\\r", '\r'} , {"\\n", '\n'}, {"\\t", '\t' }, { "\\f", '\f' }, { "\\v", '\v' } };



	E_ValueMetaType JsonIOHelper::readValueType(std::istream& stream)
	{
		if (UIOHelper::findFirstNonSpaceCharacter(stream))
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

	bool JsonIOHelper::readStringValue(std::istream& stream, std::string& value)
	{
		std::ostringstream s;
		if (UIOHelper::readNextCharacter(stream, '"'))
		{
			while (!stream.eof() && stream.peek() != '"')
			{
				unsigned char c = stream.get();
				if (c == '\\')
				{
					std::ostringstream skey;
					skey << c << static_cast<unsigned char>(stream.peek());
					std::string key = skey.str();
					if (g_escapes_read.find(key) != g_escapes_read.end())
					{
						stream.get();
						c = g_escapes_read.at(key);
					}
				}
				s << c;
			}
			if (UIOHelper::readNextCharacter(stream, '"'))
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
		return UIOHelper::tryGetNumber(value, d, t);
	}

	bool JsonIOHelper::setNonStringPrimitiveValue(std::string& primitive, double& result, E_UType& type)
	{
		if (UIOHelper::iequals(primitive, "null"))
		{
			type = E_UType::Null;
			return true;
		}
		bool b_true = UIOHelper::iequals(primitive, "true");
		if (b_true || UIOHelper::iequals(primitive, "false"))
		{
			result = (double)(b_true);
			type = E_UType::Bool;
			return true;
		}
		else
		{
			return UIOHelper::tryGetNumber(primitive, result, type);
		}

	}

	bool JsonIOHelper::readNonStringPrimitive(std::istream& stream, double& result, E_UType& type)
	{
		std::ostringstream s;
		unsigned char c;
		while (!stream.eof())
		{
			c = stream.peek();
			if (!std::isspace(c) && c != '}' && c != ']' && c != ',')
			{
				s << static_cast<unsigned char>(stream.get());
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

