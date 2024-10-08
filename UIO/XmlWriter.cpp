#include "pch.h"
#include "UIO_All.h"
#include "UIOHelper.h"
#include "XmlWriter.h"

namespace uio
{
	static void writeValue(std::ostream& stream, const UItem& value, const XmlSettings& settings, int& indentLevel, const std::string& key = "");

	const std::map<unsigned char, std::string> g_escapes = { {'<', "&lt;"},{'>',"&gt;"},{'&', "&amp;"},{'\'', "&apos;"}, {'\"', "&quot;"} };

	static std::string getSafeXmlValue(const std::string& value)
	{
		std::ostringstream s;
		for (unsigned char c : value)
		{
			if (g_escapes.find(c) != g_escapes.end())
			{
				s << g_escapes.at(c);
			}
			else
			{
				if (c>31 && c< 256)
				{
					s << c;
				}
				else
				{
					s << "&x" << std::hex << (int)c << ";";
				}
			}
		}
		return s.str();
	}

	static std::string getAlphaNumSafeXmlValue(const std::string& value)
	{
		std::ostringstream s;
		for (unsigned char c : value)
		{
			if (g_escapes.find(c) != g_escapes.end())
			{
				s << g_escapes.at(c);
			}
			else
			{
				if (std::isalnum(c) || c == ':')
				{
					s << c;
				}
				else
				{
					s << "&x" << std::hex << (int)c << ";";
				}
			}
		}
		return s.str();
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
		writePrefix(stream, prefix) << getAlphaNumSafeXmlValue(key) << "=\"" << getSafeXmlValue(value.getString()) << "\"";
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

	static void writeNonEmptyAttribute(std::ostream& stream, const std::string& key, const std::string& value)
	{
		if (!value.empty())
		{
			writeAttribute(stream, key, value, "uio");
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

	static std::string getElementNameOrType(const std::string key, E_UType type)
	{
		std::string result = UIOHelper::toPascalCase(key);
		return result.empty() ? toString(type) : result;
	}

	static void writeObject(std::ostream& stream, const UObject& object, const XmlSettings& settings, int& indentLevel, const std::string& key)
	{
		std::ostringstream markupStream;
		std::ostringstream contentStream;
		bool hasContent = false;
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(markupStream, indent, indentLevel, indentValue);
		std::string elementName = getElementNameOrType(key.empty() ? object.getClass() : key, E_UType::Object);
		beginObject(markupStream,  elementName);
		int childrenCount = 0;
		if (indentLevel == 0)
		{
			declareNameSpace(markupStream, "uio", "urn:uio:schema");
		}
		writeNonEmptyAttribute(markupStream, "key", key);
		writeNonEmptyAttribute(markupStream, "class", object.getClass());
		if (!object.isEmpty())
		{
			for (auto it = object.begin(); it != object.end(); it++)
			{
				if (it->second.isPrimitive())
				{
					writeAttribute(markupStream, it->first, it->second);
				}
				else
				{
					if (!hasContent)
					{
						hasContent = true;
						UIOHelper::handleIndent(markupStream, false, indentLevel, E_IndentMode::Increment);
					}
					writeValue(contentStream, it->second, settings, indentLevel, it->first);
				}
			}
		}
		if (hasContent)
		{
			markupStream << ">";
			UIOHelper::handleIndent(markupStream, indent, indentLevel, E_IndentMode::Decrement);
			UIOHelper::doIndent(contentStream, indent, indentLevel, indentValue);
			endObject(contentStream, elementName);
			UIOHelper::handleIndent(contentStream, indent, indentLevel, E_IndentMode::None);
			stream << markupStream.str() << contentStream.str();
		}
		else
		{
			markupStream << "/>";
			UIOHelper::handleIndent(markupStream, indent, indentLevel, E_IndentMode::None);
			stream << markupStream.str();
		}
	}

	static void writeArray(std::ostream& stream, const UArray& array, const XmlSettings& settings, int& indentLevel, const std::string& key = "")
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		std::string elementName = getElementNameOrType(key, E_UType::Array);
		beginObject(stream, elementName);
		if (indentLevel == 0)
		{
			declareNameSpace(stream, "uio", "urn:uio:schema");
		}
		writeNonEmptyAttribute(stream, "key", key);
		writeNonEmptyAttribute(stream, "type", toString(E_UType::Array));
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
			endObject(stream, elementName);
			UIOHelper::handleIndent(stream, indent, indentLevel, E_IndentMode::None);
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
		std::string elementName = getElementNameOrType(key, value.getType());
		beginObject(stream, elementName);
		if (indentLevel == 0)
		{
			declareNameSpace(stream, "uio", "urn:uio:schema");
		}
		writeNonEmptyAttribute(stream, "key", key);
		if (value.isNull())
		{
			stream << "/>";
		}
		else
		{
			stream << ">";
			if (value.isString())
			{
				stream << getSafeXmlValue(value.getString());
			}
			else
			{
				stream << value.getString();
			}
			endObject(stream, elementName);
		}
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
