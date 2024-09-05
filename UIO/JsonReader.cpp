#include "pch.h"
#include "JsonReader.h"
#include "Json.h"

namespace uio
{
	JsonReader::JsonReader()
	{
	}


	JsonReader::~JsonReader()
	{
	}

	bool readKey(std::istream& stream, std::string& key)
	{
		if (JsonIOHelper::readStringValue(stream, key))
		{
			return JsonIOHelper::readNextCharacter(stream, ':');
		}
		return false;
	}

	bool readValue(std::istream& stream, UValue& value, bool& hasNext);

	bool readProperty(UObject& object, std::istream& stream, bool& hasNext)
	{
		std::string key;
		bool isOk = false;
		if (readKey(stream, key))
		{
			isOk = readValue(stream, object[key], hasNext);
		}
		return isOk;
	}

	bool readArray(std::istream& stream, UArray& array)
	{
		array.clear();
		if (JsonIOHelper::readNextCharacter(stream, '['))
		{
			bool hasNext = false;
			bool isOk = false;
			do
			{
				array.push_back({});
				isOk = readValue(stream, array.back(), hasNext);
			} while (isOk && hasNext);
			if (!isOk)
			{
				return false;
			}
			if (JsonIOHelper::readNextCharacter(stream, ']'))
			{
				return true;
			}
		}
		return false;
	}

	bool readObject(std::istream& stream, UObject& object)
	{
		object.clear();
		if (JsonIOHelper::readNextCharacter(stream, '{'))
		{
			bool hasNext = false;
			bool isOk = false;
			do
			{
				isOk = readProperty(object, stream, hasNext);
			} while (isOk && hasNext);
			if (!isOk)
			{
				return false;
			}
			if (JsonIOHelper::readNextCharacter(stream, '}'))
			{
				return true;
			}
		}
		return false;
	}

	bool readValue(std::istream& stream, UValue& value, bool& hasNext)
	{
		bool isOk = false;
		E_ValueMetaType type = JsonIOHelper::readValueType(stream);
		switch (type)
		{
		case E_ValueMetaType::Object:
		{
			UObject o;
			if (readObject(stream, o))
			{
				value = o;
				isOk = true;
			}
			break;

		}
		case E_ValueMetaType::StringPrimitive:
		{
			std::string s;
			if (JsonIOHelper::readStringValue(stream, s))
			{
				value = s;
				isOk = true;
			}
			break;
		}
		case E_ValueMetaType::Array:
		{
			UArray a;
			if (readArray(stream, a))
			{
				value = a;
				isOk = true;
			}
			break;
		}
		case E_ValueMetaType::NonStringPrimitive:
		{
			double d;
			E_UType t;
			if (JsonIOHelper::readNonStringPrimitive(stream, d, t))
			{
				switch (t)
				{
				case E_UType::Bool: value = d == (double)1; break;
				case E_UType::Short: value = (short)d; break;
				case E_UType::Int: value = (int)d; break;
				case E_UType::Float: value = (float)d; break;
				case E_UType::Null: value = nullptr; break;
				default: value = d; break;
				}
				isOk = true;
			}
			break;
		}
		default:
			break;
		}
		if (!isOk)
		{
			return false;
		}
		hasNext = JsonIOHelper::findFirstNonSpaceCharacter(stream) && stream.peek() == ',';
		if (hasNext)
		{
			stream.get();
		}
		else
		{
			isOk = stream.peek() == '}' || stream.peek() == ']';
		}
		return isOk;
	}

	bool JsonReader::readItem(std::istream& stream, UItem& item)
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
			bool hasNext = false;
			return readValue(stream, static_cast<UValue&>(item), hasNext);
		}
	}

}
