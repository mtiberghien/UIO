#include "pch.h"
#include "UIO_All.h"
#include "XmlWriter.h"

namespace uio
{
	void writeValue(std::ostream& stream, const UItem& value, const std::string& key = "");

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

	void writeObject(std::ostream& stream, const UObject& object, const std::string& key)
	{
		stream << "<Object";
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
			stream << "/>" << std::endl;
			for (auto it = object.begin(); it != object.end(); it++)
			{
				if (!it->second.isPrimitive())
				{
					writeValue(stream, it->second, it->first);
				}
			}
			stream << "</Object>" << std::endl;
		}
		else
		{
			stream << "/>" << std::endl;
		}
	}

	void writeArray(std::ostream& stream, const UArray& array, const std::string& key = "")
	{
		stream << "<Array";
		writeKey(stream, key);
		if (!array.isEmpty())
		{
			stream << ">" << std::endl;
			for (auto it = array.begin(); it != array.end(); it++)
			{
				writeValue(stream, *it);
			}
			stream << "</Array>" << std::endl;
		}
		else
		{
			stream << "/>" << std::endl;
		}
		
	}

	void writePrimitive(std::ostream& stream, const UItem& value, const std::string& key="")
	{
		stream << "<" << toString(value.getType());
		writeKey(stream, key);
		stream << ">";
		stream << value.getString() << "</" << toString(value.getType()) << ">" << std::endl;
	}

	void writeValue(std::ostream& stream, const UItem& value, const std::string& key)
	{
		switch (value.getType())
		{
		case E_UType::Array: writeArray(stream, value.getArray(), key); break;
		case E_UType::Object: writeObject(stream, value.getObject(), key); break;
		default: writePrimitive(stream, value, key); break;
		}
	}

	void XmlWriter::writeItem(std::ostream& stream, const UItem& value)
	{
		writeValue(stream, value);
	}
}
