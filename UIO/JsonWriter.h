#pragma once
#include "UItem.h"

namespace json
{
	class JsonWriter
	{
	public:
		JsonWriter();
		~JsonWriter();
		static void writeItem(std::ostream& stream, const UItem& item, bool indent = false);
	};
}

