#pragma once
#include "UIO.h"
#include "IUSerializable.h"
#include "XmlWriter.h"
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
		static std::string serialize(const IUSerializable& object, const XmlSettings& settings = {});
		static std::string serialize(const UItem& object, const XmlSettings& settings = {});
		static bool serialize(std::ostream& stream, const IUSerializable& object, const XmlSettings& settings = {});
		static bool serialize(std::ostream& stream, const UItem& item, const XmlSettings& settings = {});
		static bool deserialize(const std::string& json, IUSerializable& object);
		static bool deserialize(const std::string& json, UItem& object);
		static bool deserialize(std::istream& stream, IUSerializable& object);
		static bool deserialize(std::istream& stream, UItem& object);
	};

}


