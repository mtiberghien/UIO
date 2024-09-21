#pragma once
#include "UItem.h"

namespace uio
{
	enum E_ClassWriteMode{Auto, Skip};

	class UIO_API JsonSettings
	{
	public:
		JsonSettings() {}
		JsonSettings(unsigned short indentValue) : m_indent(true), m_indentValue(indentValue) {}
		JsonSettings(int indentValue) : JsonSettings((unsigned short)indentValue) {}
		JsonSettings(bool indent) : m_indent(indent) {}
		JsonSettings(E_ClassWriteMode classWriteMode) : m_indent(false), m_classWriteMode(classWriteMode) {}
		bool getIndent() const { return m_indent; }
		void setIndent(bool indent) { m_indent = indent; }
		unsigned short getIndentValue() const { return m_indentValue; }
		void setIndentValue(unsigned short indentValue) { m_indentValue = indentValue; }
		E_ClassWriteMode getClassWriteMode() const { return m_classWriteMode; }
		void setClassWriteMode(E_ClassWriteMode classWriteMode) { m_classWriteMode = classWriteMode; }
	private:
		bool m_indent{ false };
		unsigned short m_indentValue{ 4 };
		E_ClassWriteMode m_classWriteMode{ E_ClassWriteMode::Auto };
	};

	class JsonWriter
	{
	public:
		JsonWriter();
		~JsonWriter();
		static void writeItem(std::ostream& stream, const UItem& item, const JsonSettings& settings);
	};
}

