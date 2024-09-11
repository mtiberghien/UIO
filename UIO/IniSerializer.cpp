#include "pch.h"
#include "IniSerializer.h"
#include "IniWriter.h"
#include "IniReader.h"

namespace uio
{
	std::string IniSerializer::serialize(const IUSerializable& object)
	{
		std::ostringstream s;
		serialize(s, object);
		return s.str();
	}

	std::string IniSerializer::serialize(const UItem& object)
	{
		std::ostringstream s;
		serialize(s, object);
		return s.str();
	}

	bool IniSerializer::serialize(std::ostream& stream, const IUSerializable& object)
	{
		UObject o;
		object.toObject(o);
		return serialize(stream, o);
	}

	bool IniSerializer::serialize(std::ostream& stream, const UItem& item)
	{
		if (stream.good())
		{
			IniWriter::writeItem(stream, item);
			return true;
		}
		return false;
	}

	bool IniSerializer::deserialize(const std::string& ini, IUSerializable& object)
	{
		std::istringstream s{ ini };
		return deserialize(s, object);
	}

	bool IniSerializer::deserialize(const std::string& ini, UItem& object)
	{
		std::istringstream s{ ini };
		return deserialize(s, object);
	}

	bool IniSerializer::deserialize(std::istream& stream, IUSerializable& object)
	{
		UObject o;
		if (deserialize(stream, o))
		{
			object.fromObject(o);
			return true;
		}
		return false;
	}

	bool IniSerializer::deserialize(std::istream& stream, UItem& object)
	{
		return IniReader::readItem(stream, object);
	}
}
