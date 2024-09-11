#pragma once
#include "UValue.h"
#include "IUSerializable.h"

namespace uio
{

	class UIO_API IniSerializer
	{
	public:
		static std::string serialize(const IUSerializable& object);
		static std::string serialize(const UItem& object);
		static bool serialize(std::ostream& stream, const IUSerializable& object);
		static bool serialize(std::ostream& stream, const UItem& item);
		static bool deserialize(const std::string& ini, IUSerializable& object);
		static bool deserialize(const std::string& ini, UItem& object);
		static bool deserialize(std::istream& stream, IUSerializable& object);
		static bool deserialize(std::istream& stream, UItem& object);
	};

}


