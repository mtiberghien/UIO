#pragma once
#include "UValue.h"

namespace uio
{
	class IniWriter
	{
	public:
		IniWriter() {}
		~IniWriter() {}
		static void writeItem(std::ostream& stream, const UItem& item);
	};
}

