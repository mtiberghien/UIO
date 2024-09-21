#include "pch.h"
#include "IniWriter.h"
#include "UIO_All.h"

namespace uio
{
	static void writeValue(std::ostream& stream, const UValue& value)
	{
		stream << value.getString();
	}

	static void writeArrayRef(std::ostream& stream, std::ostringstream& path, const std::string& key)
	{
		stream << "$A{" << path.str() << "." << key << "}";
	}

	static void writeObjectRef(std::ostream& stream, std::ostringstream& path, const std::string& key, const std::string& name)
	{
		stream << "${";
		if (!name.empty())
		{
			stream << name << ":";
		}
		stream << path.str() << "." << key << "}";
	}

	static void writeObject(std::ostream& stream, const UObject& object, std::ostringstream& path);

	static void writeArray(std::ostream& stream, const UArray& array, std::ostringstream& path)
	{
		stream << "[" << path.str() << "]" << std::endl;
		int i = 0;
		for (const auto& value : array)
		{
			stream << i << " = ";
			switch (value.getType())
			{
			case E_UType::Object: writeObjectRef(stream, path, std::to_string(i), value.getObject().getClass()); break;
			case E_UType::Array: writeArrayRef(stream, path, std::to_string(i)); break;
			default: writeValue(stream, value);  break;
			}
			stream << std::endl;
			i++;
		}
		i = 0;
		stream << std::endl;
		for (const auto& value : array)
		{
			if (value.isObject())
			{
				std::ostringstream s;
				s << path.str() << "." << std::to_string(i);
				writeObject(stream, value.getObject(), s);
			}
			else if (value.isArray())
			{
				std::ostringstream s;
				s << path.str() << "." << std::to_string(i);
				writeArray(stream, value.getArray(), s);
			}
			i++;
		}
	}

	void writeObject(std::ostream& stream, const UObject& object, std::ostringstream& path)
	{
		stream << "[" << path.str() << "]" << std::endl;
		for (const auto& kvp : object)
		{
			stream << kvp.first << " = ";
			switch (kvp.second.getType())
			{
			case E_UType::Object: writeObjectRef(stream, path, kvp.first, kvp.second.getObject().getClass()); break;
			case E_UType::Array: writeArrayRef(stream, path, kvp.first); break;
			default: writeValue(stream, kvp.second);  break;
			}
			stream << std::endl;
		}
		stream << std::endl;
		for (const auto& kvp : object)
		{
			if (kvp.second.isObject())
			{
				std::ostringstream s;
				s << path.str() << "." << kvp.first;
				writeObject(stream, kvp.second.getObject(), s);
			}
			else if (kvp.second.isArray())
			{
				std::ostringstream s;
				s << path.str() << "." << kvp.first;
				writeArray(stream, kvp.second.getArray(), s);
			}
		}
	}
	
	void IniWriter::writeItem(std::ostream& stream, const UItem& item)
	{
		std::ostringstream path;
		switch (item.getType())
		{
		case E_UType::Object:
		{
			const UObject& o = item.getObject();
			std::string root = o.getClass();
			path << (root.empty() ? toString(E_UType::Object) : root);
			writeObject(stream, o, path); break;
		}			
		case E_UType::Array:
		{
			path << toString(E_UType::Array);
			writeArray(stream, item.getArray(), path); break;
		}
		default:
			stream << "[" << path.str() << "]" << std::endl << "value = ";
			writeValue(stream, static_cast<const UValue&>(item));
			stream << std::endl;
			break;
		}
	}
}