#include "pch.h"
#include "JsonSerializer.h"
#include "UIO_All.h"
#include "JsonWriter.h"
#include "JsonReader.h"

namespace uio
{
	

	std::string JsonSerializer::serialize(const IUSerializable& object, const JsonSettings& settings)
	{

		std::ostringstream s;
		if (serialize(s, object, settings))
		{
			return s.str();
		}
		return "";
	}

	bool JsonSerializer::serialize(std::ostream& stream, const IUSerializable& object, const JsonSettings& settings)
	{
		if (stream.good())
		{
			UObject o;
			object.toObject(o);
			int indentLevel = 0;
			JsonWriter::writeItem(stream, o, settings);
			return true;
		}
		return false;
	}

	std::string JsonSerializer::serialize(const UItem& object, const JsonSettings& settings)
	{

		std::ostringstream s;
		if (serialize(s, object, settings))
		{
			return s.str();
		}
		return "";
	}

	bool JsonSerializer::serialize(std::ostream& stream, const UItem& item, const JsonSettings& settings)
	{
		if (stream.good())
		{
			int indentLevel = 0;
			JsonWriter::writeItem(stream, item, settings);
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

	std::string UJsonSerializable::serialize(const JsonSettings& settings)
	{
		std::ostringstream s;
		serialize(s, settings);
		return s.str();
	}

	bool UJsonSerializable::serialize(std::ostream& stream, const JsonSettings& settings)
	{
		return JsonSerializer::serialize(stream, *this, settings);
	}

	bool UJsonSerializable::deserialize(std::istream& stream)
	{
		return JsonSerializer::deserialize(stream, *this);
	}

	bool UJsonSerializable::deserialize(std::string& json)
	{
		std::istringstream s{ json };
		return deserialize(s);
	}
}




