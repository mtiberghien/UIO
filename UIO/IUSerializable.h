#pragma once
#include "JsonIO.h"
#include "UObject.h"

namespace json
{
	class JSONIO_API IUSerializable
	{
	public:
		IUSerializable() {};
		virtual ~IUSerializable() {};
		virtual void toObject(UObject& object) const = 0;
		virtual void fromObject(const UObject& object) = 0;

	};

	class JSONIO_API UJsonSerializable : public IUSerializable
	{
	public:
		virtual ~UJsonSerializable() {}
		std::string serialize(bool indent = false);
		bool serialize(std::ostream& stream, bool indent = false);
		bool deserialize(std::istream& stream);
		bool deserialize(std::string& string);
	};

}


