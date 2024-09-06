#pragma once
#include "UItem.h"

namespace uio
{
	class XmlReader
	{
	public:
		XmlReader() {}
		~XmlReader() {}
		static bool readItem(std::istream& stream, UItem& item);
	};

}
