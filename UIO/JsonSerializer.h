#pragma once
#include "UIO.h"
#include "IUSerializable.h"
#include "UItem.h"

namespace uio
{

	class UIO_API JsonSerializer
	{
	public:
		static std::string serialize(const IUSerializable& object, bool indent = false);
		static std::string serialize(const UItem& object, bool indent = false);
		static bool serialize(std::ostream& stream, const IUSerializable& object, bool indent = false);
		static bool serialize(std::ostream& stream, const UItem& item, bool indent = false);
		static bool deserialize(std::istream& stream, IUSerializable& object);
		static bool deserialize(const std::string& json, IUSerializable& object);
		static bool deserialize(std::istream& stream, UItem& object);
		static bool deserialize(const std::string& json, UItem& object);
	};

}


