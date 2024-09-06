#pragma once
#include "UIO.h"
#include "IUSerializable.h"
#include "UItem.h"
#include "JsonWriter.h"

namespace uio
{
	class UIO_API UJsonSerializable : public IUSerializable
	{
	public:
		virtual ~UJsonSerializable() {}
		std::string serialize(const JsonSettings& settings = {});
		bool serialize(std::ostream& stream, const JsonSettings& settings = {});
		bool deserialize(std::istream& stream);
		bool deserialize(std::string& string);
	};

	class UIO_API JsonSerializer
	{
	public:
		static std::string serialize(const IUSerializable& object, const JsonSettings& settings = {});
		static std::string serialize(const UItem& object, const JsonSettings& settings = {});
		static bool serialize(std::ostream& stream, const IUSerializable& object, const JsonSettings& settings = {});
		static bool serialize(std::ostream& stream, const UItem& item, const JsonSettings& settings = {});
		static bool deserialize(std::istream& stream, IUSerializable& object);
		static bool deserialize(const std::string& json, IUSerializable& object);
		static bool deserialize(std::istream& stream, UItem& object);
		static bool deserialize(const std::string& json, UItem& object);
	};

}


