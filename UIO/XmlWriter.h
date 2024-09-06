#pragma once
#include "UItem.h"

namespace uio
{
	class UIO_API XmlSettings
	{
	public:
		XmlSettings() : m_indent(true) {};
		XmlSettings(bool indent) : m_indent(indent) {}
		~XmlSettings() {}
		bool getIndent() const { return m_indent; }
		void setIndent(bool indent) { m_indent = indent;}
		int getIndentValue() const { return m_indentValue; }
		void setIndentValue(int indentValue) { m_indentValue = indentValue; }
	private:
		bool m_indent;
		int m_indentValue{ 2 };
	};

	class XmlWriter
	{
	public:
		XmlWriter() {}
		~XmlWriter() {}
		static void writeItem(std::ostream& stream, const UItem& item, const XmlSettings& settings = {});
	};
}

