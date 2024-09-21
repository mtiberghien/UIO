#include "pch.h"
#include "IniReader.h"
#include "UIO_All.h"
#include "UIOHelper.h"

namespace uio
{
	static bool handleComment(std::istream& stream, unsigned char c)
	{
		if (c == ';')
		{
			return false;
		}
		return true;
	}

	static bool readKey(std::istream& stream, std::string& key)
	{
		std::stringstream skey;
		while (!stream.eof())
		{
			unsigned char c = stream.get();
			if (handleComment(stream, c))
			{
				if (std::isspace(c))
				{
					if (!UIOHelper::readNextCharacter(stream, '='))
					{
						return false;
					}
					key = skey.str();
					return true;
				}
				else if (c == '=')
				{
					key = skey.str();
					return true;
				}
				else
				{
					skey << c;
				}
			}
		}
		return false;
	}

	static void readValue(std::istream& stream, std::string& value)
	{
		std::stringstream svalue;
		if (UIOHelper::findFirstNonSpaceCharacter(stream))
		{
			while (!stream.eof())
			{
				unsigned char c = stream.get();
				if (!handleComment(stream, c) || stream.eof())
				{
					break;
				}
				else
				{
					svalue << c;
				}
			}
		}
		value = svalue.str();
	}

	static bool readObjectValue(std::istream& stream, std::string& key, std::string& value)
	{
		if (UIOHelper::findFirstNonSpaceCharacter(stream))
		{
			if (stream.peek() == '[')
			{
				return false;
			}
			std::string line;
			if (std::getline(stream, line))
			{
				std::istringstream l{ line };
				if (readKey(l, key))
				{
					readValue(l, value);
					return true;
				}
			}
		}
		return false;
	}

	static bool readIniObject(std::istream& stream, UObject& parent)
	{
		UObject o;
		if (UIOHelper::readNextCharacter(stream, '['))
		{
			std::ostringstream s;
			while (!stream.eof())
			{
				unsigned char c = stream.get();
				if (c == ']')
				{
					break;
				}
				else
				{
					s << c;
				}
			}
			if (!stream.eof())
			{
				std::string l;
				if (std::getline(stream, l))
				{
					std::string key;
					std::string value;
					while (readObjectValue(stream, key, value))
					{
						UIOHelper::setValue(o[key], value);
					}
					parent[s.str()] = o;
					return true;
				}

			}
		}
		return false;
	}

	bool getReference(const std::string& value, std::string& name, std::string& path, E_UType& type)
	{
		type = E_UType::Undefined;
		std::istringstream s{ value };
		if (UIOHelper::findFirstNonSpaceCharacter(s))
		{
			if (s.peek() == '$')
			{
				s.get();
				char c;
				if (!s.eof())
				{
					c = s.get();
					if (c == 'A' && !s.eof())
					{
						type = E_UType::Array;
						c = s.get();
					}
					if (c == '{')
					{
						std::ostringstream ref;
						bool doContinue = false;
						do
						{
							c = s.get();
							doContinue = !s.eof() && c != '}';
							if (doContinue)
							{
								if (c == ':')
								{
									name = ref.str();
									ref.str("");
								}
								else
								{
									ref << c;
								}
							}
						} while (doContinue);
						path = ref.str();
						if (c == '}')
						{
							if (type == E_UType::Undefined)
							{
								type = E_UType::Object;
							}
							return true;
						}
					}
					else
					{
						type = E_UType::Undefined;
						return false;
					}
				}
			}
		}
		return false;
	}

	static bool isPath(const std::string& value)
	{
		return value.find_first_of('.') != std::string::npos;
	}

	static bool objectify(UObject& object)
	{
		std::vector<std::string> toRemove;
		for (auto& kvp : object)
		{
			if (kvp.second.isObject())
			{
				if (isPath(kvp.first))
				{
					UValue& v = object.find(kvp.first);
					if (v.isString())
					{
						std::string name;
						std::string path;
						E_UType t;
						if (getReference(v.getString(), name, path, t) && UIOHelper::iequals(path, kvp.first))
						{
							toRemove.push_back(kvp.first);
							if (t == E_UType::Array)
							{
								UArray a;
								for (const auto& aValue : kvp.second.getObject())
								{
									a.push_back(aValue.second);
								}
								v = a;
							}
							else
							{
								v = kvp.second.getObject();
								v.getObject().setClass(name);
							}
						}
					}
				}
				else if (!UIOHelper::iequals(toString(E_UType::Object), kvp.first))
				{
					kvp.second.getObject().setClass(kvp.first);
				}
			}
		}
		for (const std::string& key : toRemove)
		{
			object.erase(key);
		}
		if (object.size() == (size_t)1)
		{
			object = object[0];
		}
		return !toRemove.empty();
	}

	bool IniReader::readObject(std::istream& stream, UObject& object, const IniReaderSettings& settings)
	{
		object.clear();
		while (readIniObject(stream, object));
		if (object.isEmpty())
		{
			return false;
		}
		if (settings.getObjectify())
		{
			return objectify(object);
		}
		return true;
	}


}
