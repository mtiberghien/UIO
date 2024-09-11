#pragma once
#include "UItem.h"

namespace uio
{
	class IniReader
	{
	public:
		IniReader() {}
		~IniReader() {}
		static bool readItem(std::istream& stream, UItem& item);
	};

}
