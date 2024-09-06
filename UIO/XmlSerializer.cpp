#include "pch.h"
#include "XmlSerializer.h"
#include "UIO_All.h"
#include "XmlWriter.h"
#include "XmlReader.h"

namespace uio
{
	std::string XmlSerializer::serialize(const IUSerializable& object)
	{

		std::ostringstream s;
		if (serialize(s, object))
		{
			return s.str();
		}
		return "";
	}

	bool XmlSerializer::serialize(std::ostream& stream, const IUSerializable& object)
	{
		if (stream.good())
		{
			UObject o;
			object.toObject(o);
			int indentLevel = 0;
			XmlWriter::writeItem(stream, o);
			return true;
		}
		return false;
	}

	std::string XmlSerializer::serialize(const UItem& object)
	{

		std::ostringstream s;
		if (serialize(s, object))
		{
			return s.str();
		}
		return "";
	}

	bool XmlSerializer::serialize(std::ostream& stream, const UItem& item)
	{
		if (stream.good())
		{
			int indentLevel = 0;
			XmlWriter::writeItem(stream, item);
			return true;
		}
		return false;
	}

	bool XmlSerializer::deserialize(const std::string& json, IUSerializable& object)
	{
		std::istringstream s{ json };
		return deserialize(s, object);
	}

	bool XmlSerializer::deserialize(std::istream& stream, IUSerializable& object)
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

	bool XmlSerializer::deserialize(const std::string& json, UItem& object)
	{
		std::istringstream s{ json };
		return deserialize(s, object);
	}

	bool XmlSerializer::deserialize(std::istream& stream, UItem& object)
	{
		return XmlReader::readItem(stream, object);
	}

	std::string UXmlSerializable::serialize()
	{
		std::ostringstream s;
		serialize(s);
		return s.str();
	}

	bool UXmlSerializable::serialize(std::ostream& stream)
	{
		return XmlSerializer::serialize(stream, *this);
	}

	bool UXmlSerializable::deserialize(std::istream& stream)
	{
		return XmlSerializer::deserialize(stream, *this);
	}

	bool UXmlSerializable::deserialize(std::string& json)
	{
		std::istringstream s{ json };
		return deserialize(s);
	}
}
