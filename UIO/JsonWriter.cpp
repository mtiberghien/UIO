#include "pch.h"
#include "JsonWriter.h"
#include "Json.h"

namespace uio
{
	JsonWriter::JsonWriter()
	{
	}


	JsonWriter::~JsonWriter()
	{
	}

	void writeValue(std::ostream& stream, const UItem& value, bool indent, int& indentLevel);

	void writeProperty(std::ostream& stream, const std::string& key, const UValue& value, bool indent, int& indentLevel)
	{
		JsonIOHelper::doIndent(stream, indent, indentLevel) << '"' << key << '"' << ": ";
		if ((value.isObject() || value.isArray()) && value.getInt() > 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
			if (value.getType() == E_UType::Object)
			{
				JsonIOHelper::doIndent(stream, indent, indentLevel);
			}
		}
		writeValue(stream, value, indent, indentLevel);
	}

	void writeObject(std::ostream& stream, const UObject& object, bool indent, int& indentLevel)
	{
		stream << "{";
		if (!object.isEmpty())
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (auto it = object.begin(); it != object.end(); it++)
			{
				writeProperty(stream, it->first, it->second, indent, indentLevel);
				if (it != std::prev(object.end()))
				{
					stream << ", ";
					JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "}";
		if (indentLevel == 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	void writeArray(std::ostream& stream, const UArray& array, bool indent, int& indentLevel)
	{
		if (!array.isEmpty())
		{
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "[";
		if (!array.isEmpty())
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Increment);
			for (auto it = array.begin(); it != array.end(); it++)
			{
				JsonIOHelper::doIndent(stream, indent, indentLevel);
				writeValue(stream, *it, indent, indentLevel);
				if (it != std::prev(array.end()))
				{
					stream << ", ";
					JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
				}

			}
			JsonIOHelper::handleIndent(stream, indent, indentLevel, Decrement);
			JsonIOHelper::doIndent(stream, indent, indentLevel);
		}
		stream << "]";
		if (indentLevel == 0)
		{
			JsonIOHelper::handleIndent(stream, indent, indentLevel, None);
		}
	}

	void writeValue(std::ostream& stream, const UItem& value, bool indent, int& indentLevel)
	{
		switch (value.getType())
		{
		case E_UType::Array: writeArray(stream, value.getArray(), indent, indentLevel); break;
		case E_UType::Object: writeObject(stream, value.getObject(), indent, indentLevel); break;
		case E_UType::String:
			stream << '"';
			for (char c : value.getString())
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

	void JsonWriter::writeItem(std::ostream& stream, const UItem& value, bool indent)
	{
		int indentLevel = 0;
		writeValue(stream, value, indent, indentLevel);
	}
}

