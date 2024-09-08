#include "pch.h"
#include "XmlReader.h"
#include "UIO_All.h"
#include "UIOHelper.h"

namespace uio
{
	enum E_MarkupType{System, SystemData, Comment, Normal};

	const std::map< std::string, char> g_escapes_read = { {"lt", '<'},{"gt", '>'},{"amp", '&'},{"apos", '\''}, {"quot", '\"'} };

	static E_MarkupType getMarkupType(std::istream& stream)
	{
		char c = stream.peek();
		switch (c)
		{
		case '?': stream.get();
			return E_MarkupType::System;
		case '!': 
		{
			c = stream.get();
			auto pos = stream.tellg();
			if (stream.peek() == '-')
			{
				c = stream.get();
				if (stream.peek() == '-')
				{
					return E_MarkupType::Comment;
				}
				stream.seekg(pos);
			}		
			return E_MarkupType::SystemData;
		}
		default: return E_MarkupType::Normal;
		}
	}

	static bool findMarkup(std::istream& stream)
	{
		if (UIOHelper::readNextCharacter(stream, '<'))
		{
			return UIOHelper::findFirstNonSpaceCharacter(stream);
		}
		return false;
	}

	static bool readMarkup(std::istream& stream, UItem& value)
	{
		return false;
	}

	static void skipComment(std::istream& stream)
	{
		if (UIOHelper::readNextCharacter(stream, '-'))
		{
			if (stream.peek() == '-')
			{
				char c = stream.get();
				if (stream.peek() != '>')
				{
					skipComment(stream);
				}
				else
				{
					stream.get();
				}
			}
		}
	}

	static void skipSystemMarkup(std::istream& stream)
	{
		if (UIOHelper::readNextCharacter(stream, '?'))
		{
			if (stream.peek() == '>')
			{
				stream.get();
			}
			else
			{
				skipSystemMarkup(stream);
			}
		}
	}

	static void skipSystemDataMarkup(std::istream& stream)
	{
		UIOHelper::readNextCharacter(stream, '>');
	}

	static bool getFirstNormalMarkup(std::istream& stream)
	{
		if(findMarkup(stream))
		{
			E_MarkupType t = getMarkupType(stream);
			switch (t)
			{
			case E_MarkupType::Normal: return true;
			case E_MarkupType::Comment:
				skipComment(stream);
				break;
			case E_MarkupType::System:
				skipSystemMarkup(stream);
				break;
			case E_MarkupType::SystemData:
				skipSystemDataMarkup(stream);
				break;
			}
			return getFirstNormalMarkup(stream);
		}
		return false;
	}

	static bool readSpecial(std::istream& stream, char& c)
	{
		std::string special;
		if (std::getline(stream, special, ';'))
		{
			if (g_escapes_read.find(special) != g_escapes_read.end())
			{
				c = g_escapes_read.at(special);
				return true;
			}
			else
			{
				int c_code;
				if (std::tolower(special[0]) == 'x')
				{
					std::istringstream s{ special.substr(1) };
					s >> std::hex >> c_code;
				}
				else
				{
					std::istringstream s{ special};
					s >> std::dec >> c_code;
				}
				c = (char)c_code;
				return true;
			}
		}
		return false;
	}

	static bool readKey(std::istream& stream, std::string& prefix, std::string& key)
	{
		std::ostringstream buffer;
		while (!stream.eof())
		{
			char c = stream.get();
			if (!std::isspace(c))
			{
				switch (c)
				{
				case ':':
					prefix == buffer.str();
					buffer.clear();
					break;
				case '=':
					key = buffer.str();
					return true;
				case '&':
					if (readSpecial(stream, c))
					{
						buffer << c;
						break;
					}
					else
					{
						return false;
					}
				default:
					buffer << c;
					break;
				}
			}

		}
		return false;
	}

	static bool readValue(std::istream&, std::string& value)
	{
		return false;
	}

	static void setValue(UValue& uValue, const std::string& sValue)
	{
		E_UType t;
		double d;
		if (UIOHelper::tryGetNumber(sValue, d, t))
		{
			switch (t)
			{
			case E_UType::Bool: uValue = d == (double)1; break;
			case E_UType::Short: uValue = (short)d; break;
			case E_UType::Int: uValue = (int)d; break;
			case E_UType::Float: uValue = (float)d; break;
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

	static bool readAttributes(std::istream& stream, UObject& object, bool& hasChildren)
	{
		if (UIOHelper::findFirstNonSpaceCharacter(stream))
		{
			std::string prefix{ "" };
			std::string key{ "" };
			std::string value{ "" };
			if (readKey(stream, prefix, key) && readValue(stream, value))
			{
				if (prefix.empty())
				{
					setValue(object[key], value);
				}
			}
		}
		return false;
	}

	static bool readObject(std::istream& stream, UObject& object)
	{
		if (getFirstNormalMarkup(stream))
		{
			std::string elementName = UIOHelper::readWordLowerCase(stream);
			bool hasChildren;
			if (readAttributes(stream, object, hasChildren))
			{

			}
			else
			{
				return false;
			}
		}
		return false;
	}

	static bool readArray(std::istream& stream, UArray& array)
	{
		return false;
	}

	static bool readValue(std::istream& stream, UValue& value)
	{
		return false;
	}

	bool XmlReader::readItem(std::istream& stream, UItem& item)
	{
		if (item.isObject())
		{
			return readObject(stream, item.getObject());
		}
		else if (item.isArray())
		{
			return readArray(stream, item.getArray());
		}
		else
		{
			return readValue(stream, static_cast<UValue&>(item));
		}
	}
}