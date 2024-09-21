#include "pch.h"
#include "XmlReader.h"
#include "UIO_All.h"
#include "UIOHelper.h"

namespace uio
{
	enum E_MarkupType{System, SystemData, Comment, BeginMarkup, EndMarkup};

	const std::map< std::string, unsigned char> g_escapes_read = { {"lt", '<'},{"gt", '>'},{"amp", '&'},{"apos", '\''}, {"quot", '\"'} };

	static E_MarkupType getMarkupType(std::istream& stream)
	{
		unsigned char c = stream.peek();
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
		case '/': stream.get();
			return E_MarkupType::EndMarkup;
		default: return E_MarkupType::BeginMarkup;
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

	static void skipComment(std::istream& stream)
	{
		if (UIOHelper::readNextCharacter(stream, '-'))
		{
			if (stream.peek() == '-')
			{
				unsigned char c = stream.get();
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

	static bool getFirstBeginMarkup(std::istream& stream, E_MarkupType& type)
	{
		if(findMarkup(stream))
		{
			type = getMarkupType(stream);
			switch (type)
			{
			case E_MarkupType::BeginMarkup: return true;
			case E_MarkupType::EndMarkup: return false;
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
			return getFirstBeginMarkup(stream, type);
		}
		return false;
	}

	static bool readSpecial(std::istream& stream, unsigned char& c)
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
				c = static_cast<unsigned char>(c_code);
				return true;
			}
		}
		return false;
	}

	static bool readSafeChar(std::istream& stream, unsigned char& c)
	{
		switch (c)
		{
		case '&':
			return readSpecial(stream, c);
		default:
			return true;
		}
	}

	static bool readAttributeKey(std::istream& stream, std::string& prefix, std::string& key)
	{
		std::ostringstream buffer;
		while (!stream.eof())
		{
			unsigned char c = stream.get();
			if (!std::isspace(c))
			{
				switch (c)
				{
				case ':':
					prefix = buffer.str();
					buffer.str("");
					break;
				case '=':
					key = buffer.str();
					return true;
				default:
					if (readSafeChar(stream, c))
					{
						buffer << c;
					}
					else
					{
						return false;
					}
					break;
				}
			}

		}
		return false;
	}

	static bool readAttributeValue(std::istream& stream, std::string& value)
	{
		if (UIOHelper::readNextCharacter(stream, '\"'))
		{
			std::ostringstream s;
			while (!stream.eof())
			{
				unsigned char c = stream.get();
				if (readSafeChar(stream, c))
				{
					if (c == '\"')
					{
						value = s.str();
						return true;
					}
					else
					{
						s << c;
					}
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}

	

	static bool readAttributes(std::istream& stream, UItem& item, bool& hasContent, std::string& key)
	{
		bool isArray = false;
		while (UIOHelper::findFirstNonSpaceCharacter(stream))
		{
			if (stream.peek() == '/')
			{
				stream.get();
				if (stream.peek() == '>')
				{
					hasContent = false;
					return true;
				}
			}
			else  if (stream.peek() == '>')
			{
				stream.get();
				hasContent = true;
				if (isArray && item.isUndefined())
				{
					((UValue&)item) = E_UType::Array;
				}
				return true;
			}
			std::string prefix{ "" };
			std::string attributeKey{ "" };
			std::string value{ "" };
			if (readAttributeKey(stream, prefix, attributeKey) && readAttributeValue(stream, value))
			{
				bool isUIOSchema = UIOHelper::iequals("uio", prefix);
				bool isUIOClass = isUIOSchema && UIOHelper::iequals("class", attributeKey);
				bool isAttribute = prefix.empty();
				bool shouldBeObject = isAttribute || isUIOClass;
				if (shouldBeObject && item.isUndefined())
				{
					((UValue&)item) = E_UType::Object;

				}
				if (item.isObject())
				{
					if (isAttribute)
					{
						UIOHelper::setValue(item[attributeKey], value);
					}
					if (isUIOClass)
					{
						item.getObject().setClass(value);
					}
				}
				if (isUIOSchema)
				{
					if (UIOHelper::iequals("key", attributeKey))
					{
						key = value;
					}
					else if (UIOHelper::iequals("type", attributeKey))
					{
						isArray = UIOHelper::iequals(toString(E_UType::Array), value);
					}
					
				}
			}
		}
		return false;
	}

	static std::string getElementName(std::istream& stream)
	{
		std::ostringstream s;
		while (!stream.eof())
		{
			unsigned char c = stream.peek();
			if ( std::isspace(c) || c == '>' || c == '/')
			{
				return s.str();
			}
			else
			{
				c = stream.get();
				s << c;
			}
		}
		return s.str();
	}

	static std::string getValueContent(std::istream& stream)
	{
		std::ostringstream s;
		while (!stream.eof())
		{
			unsigned char c = stream.peek();
			if (c != '<')
			{
				c = stream.get();
				s << c;
			}
			else
			{
				return s.str();
			}
		}
		return s.str();
	}

	static bool changeToArray(UItem& item)
	{
		UValue* ptrV = dynamic_cast<UValue*>(&item);
		if (ptrV)
		{
			UObject o = item.getObject();
			UArray a;
			for (const auto& v : o)
			{
				a << v.second;
			}
			*ptrV = a;
			return true;
		}
		return false;
	}

	static bool readMarkupItem(std::istream& stream, UItem& item, std::string& key, E_MarkupType& foundType)
	{
		if (getFirstBeginMarkup(stream, foundType))
		{
			std::string elementName = getElementName(stream);
			bool hasContent;
			if (readAttributes(stream, item, hasContent, key))
			{
				key = key.empty() ? elementName : key;
				if(item.isObject())
				{
					UObject& o = item.getObject();
					if (o.getClass().empty() && !UIOHelper::iequals(toString(E_UType::Object), elementName))
					{
						o.setClass(UIOHelper::toLower(elementName));
					}			
				}
				if (hasContent)
				{
					if (UIOHelper::findFirstNonSpaceCharacter(stream))
					{
						if (stream.peek() == '<')
						{
							if (item.isUndefined())
							{
								((UValue&)item) = E_UType::Object;
							}
							bool hasNext = false;
							do
							{
								std::string childKey;
								UValue child;
								hasNext = readMarkupItem(stream, child, childKey, foundType);
								if (!hasNext)
								{
									if (foundType == E_MarkupType::EndMarkup)
									{
										std::string endElementName = getElementName(stream);
										UIOHelper::readNextCharacter(stream, '>');
										return elementName == endElementName;
									}
									return false;
								}
								if (item.isObject())
								{
									if (!item.getObject().exists(childKey))
									{
										item[childKey] = child;
									}
									else
									{
										if (changeToArray(item))
										{
											item.getArray().push_back(child);
										}
										else return false;
									}
								}
								else if (item.isArray())
								{
									item.getArray().push_back(child);
								}
							} while (hasNext);
						}
						else
						{
							std::string value = getValueContent(stream);
							if (item.isUndefined())
							{
								UIOHelper::setValue((UValue&)item, value, fromString(elementName));
								return !getFirstBeginMarkup(stream, foundType) && foundType == E_MarkupType::EndMarkup && getElementName(stream) == elementName && UIOHelper::readNextCharacter(stream, '>');

							}
							return false;
						}
					}
					
				}
				else
				{
					if (fromString(elementName) == E_UType::Null)
					{
						((UValue&)item) = nullptr;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool XmlReader::readItem(std::istream& stream, UItem& item)
	{
		std::string key{ "" };
		E_MarkupType lastMarkupFound;
		return readMarkupItem(stream, item, key, lastMarkupFound);
	}
}