#include "pch.h"
#include "IUSerializable.h"
#include "UValue.h"
#include "JsonSerializer.h"

namespace uio
{

	std::string UJsonSerializable::serialize(bool indent)
	{
		std::ostringstream s;
		serialize(s, indent);
		return s.str();
	}

	bool UJsonSerializable::serialize(std::ostream& stream, bool indent)
	{
		return JsonSerializer::serialize(stream, *this, indent);
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


