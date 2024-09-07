#include "pch.h"
#include "UIO_All.h"
#include "UIOHelper.h"
#include "XmlWriter.h"

namespace uio
{
	static void writeValue(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key = "");

	static std::string getSafeXmlValue(const std::string& value)
	{
		//Todo handle special characters
		return value;
	}

	static std::ostream& writePrefix(std::ostream& stream, const std::string& prefix)
	{
		if (!prefix.empty())
		{
			stream << prefix << ":";
		}

		return stream;
	}

	static void writeAttribute(std::ostream& stream, const std::string& key, const UValue& value, const std::string& prefix="")
	{
		stream << " ";
		writePrefix(stream, prefix) << key << "=\"" << getSafeXmlValue(value.getString()) << "\"";
	}

	static void beginObject(std::ostream& stream, const std::string& name, const std::string& prefix="")
	{
		stream << "<";
		writePrefix(stream, prefix) << name;
	}

	static void endObject(std::ostream& stream, const std::string& name, const std::string& prefix="")
	{
		stream << "</";
		writePrefix(stream, prefix) << name << ">";
	}

	static void writeKey(std::ostream& stream, const std::string& key)
	{
		if (!key.empty())
		{
			writeAttribute(stream, "key", key, "uio");
		}
	}

	static void declareNameSpace(std::ostream& stream, const std::string& prefix, const std::string& namespaceValue)
	{
		std::ostringstream s;
		s << "xmlns";
		if (!prefix.empty())
		{
			s << ":" << prefix;
		}
		writeAttribute(stream, s.str(), namespaceValue);
	}

	static void writeObject(std::ostream& stream, const UObject& object, const XmlSettings& settings, int& indentLevel, const std::string& key)
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		beginObject(stream, toString(E_UType::Object));
		int childrenCount = 0;
		if (indentLevel == 0)
		{
			declareNameSpace(stream, "uio", "urn:uio:schema");
		}
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
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
			endObject(stream, toString(E_UType::Object));
		}
		else
		{
			stream << "/>";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
		}
	}

	static void writeArray(std::ostream& stream, const UArray& array, const XmlSettings& settings, int& indentLevel, const std::string& key = "")
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		beginObject(stream, toString(E_UType::Array));
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
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
			endObject(stream, toString(E_UType::Array));
		}
		else
		{
			stream << "/>";
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
		}
		
	}

	static void writePrimitive(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key="")
	{
		bool indent = settings.getIndent();
		UIOHelper::doIndent(stream, indent, indentLevel, settings.getIndentValue());
		beginObject(stream, toString(value.getType()));
		writeKey(stream, key);
		stream << ">";
		stream << value.getString();
		endObject(stream, toString(value.getType()));
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
