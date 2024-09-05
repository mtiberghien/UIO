#pragma once
#include "UItem.h"

namespace json
{
	class JsonReader
	{
	public:
		JsonReader();
		~JsonReader();
		static bool readItem(std::istream& stream, UItem& item);
	};

}

