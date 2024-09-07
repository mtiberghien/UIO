#include "pch.h"
#include "UIO_All.h"
#include "UIOHelper.h"
#include "XmlWriter.h"

namespace uio
{
	void writeValue(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key = "");

	std::string getSafeXmlValue(const std::string& value)
	{
		//Todo handle special characters
		return value;
	}

	void writeAttribute(std::ostream& stream, const std::string& key, const UValue& value)
	{
		stream << " " << key << "=\"" << getSafeXmlValue(value.getString()) << "\"";
	}

	void writeKey(std::ostream& stream, const std::string& key)
	{
		if (!key.empty())
		{
			writeAttribute(stream, "key", key);
		}
	}

	void writeObject(std::ostream& stream, const UObject& object, const XmlSettings& settings, int& indentLevel, const std::string& key)
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue) << "<Object";
		int childrenCount = 0;
		writeKey(stream, key);
		if (!object.isEmpty())
		{
			for (auto it = object.begin(); it != object.end(); it++)
			{
				if (it->second.isPrimitive())
				{
					writeAttribute(stream, it->first, it->second);
				}
				else
				{
					childrenCount++;
				}
			}
		}
		if (childrenCount>0)
		{
			stream << "/>";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::Increment);
			for (auto it = object.begin(); it != object.end(); it++)
			{
				if (!it->second.isPrimitive())
				{
					writeValue(stream, it->second, settings, indentLevel, it->first);
				}
			}
			UIOHelper::handleIndent(stream, true, indentLevel, E_IndentMode::Decrement);
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue) << "</Object>";
		}
		else
		{
			stream << "/>";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
		}
	}

	void writeArray(std::ostream& stream, const UArray& array, const XmlSettings& settings, int& indentLevel, const std::string& key = "")
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue) << "<Array";
		writeKey(stream, key);
		if (!array.isEmpty())
		{
			stream << ">";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::Increment);
			for (auto it = array.begin(); it != array.end(); it++)
			{
				writeValue(stream, *it, settings, indentLevel);
			}
			UIOHelper::handleIndent(stream, false, indentLevel, E_IndentMode::Decrement);
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue) << "</Array>";
		}
		else
		{
			stream << "/>";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
		}
		
	}

	void writePrimitive(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key="")
	{
		bool indent = settings.getIndent();
		UIOHelper::doIndent(stream, indent, indentLevel, settings.getIndentValue());
		stream << "<" << toString(value.getType());
		writeKey(stream, key);
		stream << ">";
		stream << value.getString() << "</" << toString(value.getType()) << ">";
		UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
	}

	void writeValue(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key)
	{
		switch (value.getType())
		{
		case E_UType::Array: writeArray(stream, value.getArray(), settings, indentLevel, key); break;
		case E_UType::Object: writeObject(stream, value.getObject(), settings, indentLevel, key); break;
		default: writePrimitive(stream, value, settings, indentLevel, key); break;
		}
	}

	void XmlWriter::writeItem(std::ostream& stream, const UItem& value, const XmlSettings& settings)
	{
		int indentLevel = 0;
		writeValue(stream, value, settings, indentLevel);
	}
}
