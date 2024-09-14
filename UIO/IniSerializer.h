#pragma once
#include "UValue.h"
#include "IUSerializable.h"
#include "IniReader.h"

namespace uio
{

	class UIO_API IniSerializer
	{
	public:
		static std::string serialize(const IUSerializable& object);
		static std::string serialize(const UItem& object);
		static bool serialize(std::ostream& stream, const IUSerializable& object);
		static bool serialize(std::ostream& stream, const UItem& item);
		static bool deserialize(const std::string& ini, IUSerializable& object, const IniReaderSettings& settings = {});
		static bool deserialize(const std::string& ini, UObject& object, const IniReaderSettings& settings = {});
		static bool deserialize(std::istream& stream, IUSerializable& object, const IniReaderSettings& settings = {});
		static bool deserialize(std::istream& stream, UObject& object, const IniReaderSettings& settings = {});
	};

}


