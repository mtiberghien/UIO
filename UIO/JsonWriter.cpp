#include "pch.h"
#include "JsonWriter.h"
#include "UIO_All.h"

namespace uio
{
	JsonWriter::JsonWriter()
	{
	}


	JsonWriter::~JsonWriter()
	{
	}

	const std::map<unsigned char, std::string> g_escapes = { {'\r', "\\r"}, {'\n', "\\n"},{'\t', "\\t"},{'\f',"\\f"},{'\v', "\\v"} };

	static void writeValue(std::ostream& stream, const UItem& value, const JsonSettings& settings, int& indentLevel);

	static void writeProperty(std::ostream& stream, const std::string& key, const UValue& value, const JsonSettings& settings, int& indentLevel)
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		UIOHelper::doIndent(stream, indent, indentLevel, indentValue) << '"' << key << '"' << ": ";
		if ((value.isObject() || value.isArray()) && value.getInt() > 0)
		{
			UIOHelper::handleIndent(stream, indent, indentLevel, None);
			if (value.getType() == E_UType::Object)
			{
				UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
			}
		}
		writeValue(stream, value, settings, indentLevel);
	}

	static void writeObject(std::ostream& stream, const UObject& object, const JsonSettings& settings, int& indentLevel)
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		stream << "{";
		if (!object.isEmpty())
		{
			UIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (auto it = object.begin(); it != object.end(); it++)
			{
				writeProperty(stream, it->first, it->second, settings, indentLevel);
				if (it != std::prev(object.end()))
				{
					stream << ", ";
					UIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			UIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		}
		stream << "}";
		if (indentLevel == 0)
		{
			UIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	static void writeArray(std::ostream& stream, const UArray& array, const JsonSettings& settings, int& indentLevel)
	{
		bool indent = settings.getIndent();
		unsigned short indentValue = settings.getIndentValue();
		if (!array.isEmpty())
		{
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		}
		stream << "[";
		if (!array.isEmpty())
		{
			UIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (auto it = array.begin(); it != array.end(); it++)
			{
				UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
				writeValue(stream, *it, settings, indentLevel);
				if (it != std::prev(array.end()))
				{
					stream << ", ";
					UIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			UIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			UIOHelper::doIndent(stream, indent, indentLevel, indentValue);
		}
		stream << "]";
		if (indentLevel == 0)
		{
			UIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	void writeValue(std::ostream& stream, const UItem& value, const JsonSettings& settings, int& indentLevel)
	{
		switch (value.getType())
		{
		case E_UType::Array: writeArray(stream, value.getArray(), settings, indentLevel); break;
		case E_UType::Object: writeObject(stream, value.getObject(), settings, indentLevel); break;
		case E_UType::String:
			stream << '"';
			for (unsigned char c : value.getString())
			{
				if (g_escapes.find(c) != g_escapes.end())
				{
					stream << g_escapes.at(c);
				}
				else
				{
					stream << c;
				}
			}
			stream << '"';
			break;
		default:
			stream << value.getString();
		}
	}

	void JsonWriter::writeItem(std::ostream& stream, const UItem& value, const JsonSettings& settings)
	{
		int indentLevel = 0;
		writeValue(stream, value, settings, indentLevel);
	}
}

