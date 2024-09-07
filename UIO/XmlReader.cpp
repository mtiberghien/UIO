#include "pch.h"
#include "XmlReader.h"
#include "UIO_All.h"
#include "UIOHelper.h"

namespace uio
{
	enum E_MarkupType{System, SystemData, Comment, Normal};

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
		E_MarkupType t = getMarkupType(stream);
		switch (t)
		{

		}
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

	static bool readObject(std::istream& stream, UObject& object)
	{
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