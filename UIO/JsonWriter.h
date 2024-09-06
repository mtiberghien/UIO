#pragma once
#include "UItem.h"

namespace uio
{
	class UIO_API JsonSettings
	{
	public:
		JsonSettings() :m_indent(false) {}
		JsonSettings(bool indent) : m_indent(indent) {}
		void setIndent(bool indent) { m_indent = indent; }
		bool getIndent() const { return m_indent; }
	private:
		bool m_indent{ false };
	};

	class JsonWriter
	{
	public:
		JsonWriter();
		~JsonWriter();
		static void writeItem(std::ostream& stream, const UItem& item, const JsonSettings& settings);
	};
}

