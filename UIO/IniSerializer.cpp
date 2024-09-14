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

	bool IniSerializer::deserialize(const std::string& ini, IUSerializable& object, const IniReaderSettings& settings)
	{
		std::istringstream s{ ini };
		return deserialize(s, object, settings);
	}

	bool IniSerializer::deserialize(const std::string& ini, UObject& object, const IniReaderSettings& settings)
	{
		std::istringstream s{ ini };
		return deserialize(s, object, settings);
	}

	bool IniSerializer::deserialize(std::istream& stream, IUSerializable& object, const IniReaderSettings& settings)
	{
		UObject o;
		if (deserialize(stream, o, settings))
		{
			object.fromObject(o);
			return true;
		}
		return false;
	}

	bool IniSerializer::deserialize(std::istream& stream, UObject& object, const IniReaderSettings& settings)
	{
		return IniReader::readObject(stream, object, settings);
	}
}
