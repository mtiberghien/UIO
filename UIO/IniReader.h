#pragma once
#include "UIO.h"
#include "UItem.h"

namespace uio
{
	class UIO_API IniReaderSettings
	{
	public:
		IniReaderSettings(bool objectify): m_objectify(objectify){}
		IniReaderSettings() : IniReaderSettings(true) {}
		~IniReaderSettings() {}
		bool getObjectify() const { return m_objectify; }
		void setObjectify(bool objectify) { m_objectify = objectify; }
	private:
		bool m_objectify;
	};
	class IniReader
	{
	public:
		IniReader() {}
		~IniReader() {}
		static bool readObject(std::istream& stream, UObject& object, const IniReaderSettings& settings = {});
	};

}
