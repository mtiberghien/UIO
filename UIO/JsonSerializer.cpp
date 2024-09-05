#include "pch.h"
#include "JsonSerializer.h"
#include "Json.h"
#include "JsonWriter.h"
#include "JsonReader.h"

namespace json
{
	

	std::string JsonSerializer::serialize(const IUSerializable& object, bool indent)
	{

		std::ostringstream s;
		if (serialize(s, object, indent))
		{
			return s.str();
		}
		return "";
	}

	bool JsonSerializer::serialize(std::ostream& stream, const IUSerializable& object, bool indent)
	{
		if (stream.good())
		{
			UObject o;
			object.toObject(o);
			int indentLevel = 0;
			JsonWriter::writeItem(stream, o, indent);
			return true;
		}
		return false;
	}

	std::string JsonSerializer::serialize(const UItem& object, bool indent)
	{

		std::ostringstream s;
		if (serialize(s, object, indent))
		{
			return s.str();
		}
		return "";
	}

	bool JsonSerializer::serialize(std::ostream& stream, const UItem& item, bool indent)
	{
		if (stream.good())
		{
			int indentLevel = 0;
			JsonWriter::writeItem(stream, item, indent);
			return true;
		}
		return false;
	}

	bool JsonSerializer::deserialize(const std::string& json, IUSerializable& object)
	{
		std::istringstream s{ json };
		return deserialize(s, object);
	}

	bool JsonSerializer::deserialize(std::istream& stream, IUSerializable& object)
	{
		if (stream.good())
		{
			UObject o;
			if (deserialize(stream, o))
			{
				object.fromObject(o);
				return true;
			}
		}

		return false;
	}

	bool JsonSerializer::deserialize(const std::string& json, UItem& object)
	{
		std::istringstream s{ json };
		return deserialize(s, object);
	}

	bool JsonSerializer::deserialize(std::istream& stream, UItem& object)
	{
		return JsonReader::readItem(stream, object);
	}
}




