#pragma once
#include "UIO.h"
#include "IUSerializable.h"
#include "UItem.h"

namespace uio
{

	class UIO_API UXmlSerializable : public IUSerializable
	{
	public:
		virtual ~UXmlSerializable() {}
		std::string serialize();
		bool serialize(std::ostream& stream);
		bool deserialize(std::istream& stream);
		bool deserialize(std::string& string);
	};

	class UIO_API XmlSerializer
	{
	public:
		static std::string serialize(const IUSerializable& object);
		static std::string serialize(const UItem& object);
		static bool serialize(std::ostream& stream, const IUSerializable& object);
		static bool serialize(std::ostream& stream, const UItem& item);
		static bool deserialize(const std::string& json, IUSerializable& object);
		static bool deserialize(const std::string& json, UItem& object);
		static bool deserialize(std::istream& stream, IUSerializable& object);
		static bool deserialize(std::istream& stream, UItem& object);
	};

}


