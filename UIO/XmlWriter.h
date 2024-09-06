#pragma once
#include "UItem.h"

namespace uio
{
	class XmlWriter
	{
	public:
		XmlWriter() {}
		~XmlWriter() {}
		static void writeItem(std::ostream& stream, const UItem& item);
	};
}

