#pragma once
#include "UItem.h"

namespace uio
{
	class UIO_API XmlSettings
	{
	public:
		XmlSettings() {};
		XmlSettings(unsigned short indentValue) : m_indent(true), m_indentValue(indentValue) {}
		XmlSettings(int indentValue) : XmlSettings((unsigned short)indentValue) {}
		XmlSettings(bool indent) : m_indent(indent) {}
		~XmlSettings() {}
		bool getIndent() const { return m_indent; }
		void setIndent(bool indent) { m_indent = indent;}
		unsigned short getIndentValue() const { return m_indentValue; }
		void setIndentValue(unsigned short indentValue) { m_indentValue = indentValue; }
	private:
		bool m_indent{ true };
		unsigned short m_indentValue{ 2 };
	};

	class XmlWriter
	{
	public:
		XmlWriter() {}
		~XmlWriter() {}
		static void writeItem(std::ostream& stream, const UItem& item, const XmlSettings& settings = {});
	};
}

