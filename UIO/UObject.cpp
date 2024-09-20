#include "pch.h"
#include "UObject.h"
#include "UValue.h"
#include "UArray.h"

namespace uio
{
	UObject::UObject(const std::string& name, std::initializer_list < std::pair<const std::string, UValue>> properties): m_name(name), m_properties(properties)
	{
	}

	UObject& UObject::operator=(const UObject& object)
	{
		UObject o = object;
		clear();
		for (const auto& it : o.m_properties)
		{
			this->operator [](it.first) = it.second;
		}
		this->setName(o.getName());
		return *this;
	}

	UValue& UObject::operator[](const std::string& key) {
		if (!exists(key))
		{
			m_properties[key] = UValue{};
		}
		return m_properties[key];
	}

	const UValue& UObject::operator[](const std::string& key) const
	{
		if (!exists(key))
		{
			return UUndefinedProvider::getError();
		}
		return m_properties.at(key);
	}

	UValue& UObject::getOrError(const std::string& key)
	{
		if (!exists(key))
		{
			return UUndefinedProvider::getError();
		}
		return m_properties[key];
	}

	UValue& UObject::operator[](int index)
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return it->second;
		}
		return UUndefinedProvider::getError();
	}

	bool UObject::erase(const std::string& key)
	{
		return m_properties.erase(key) == 1;
	}

	bool UObject::eraseAt(int index)
	{
		if (index >= 0 && index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			m_properties.erase(it);
			return true;
		}
		return false;
	}

	const UValue& UObject::operator[](int index) const
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return m_properties.at(it->first);
		}
		return UUndefinedProvider::getError();
	}

	std::vector<std::string> UObject::getKeys() const
	{
		std::vector<std::string> result;
		for (auto& p : m_properties)
		{
			result.push_back(p.first);
		}
		return result;
	}

	std::string UObject::getString(const std::string& defaultValue) const
	{
		std::ostringstream s;
		s << "Object";
		if (!m_name.empty())
		{
			s << ":" << m_name;
		}
		s << "{";
		for (auto it = begin(); it != end(); it++)
		{
			s << it->first << ": " << toString(it->second.getType());
			if (it != std::prev(end()))
			{
				s << ", ";
			}
		}
		s << "}";
		return s.str();
	}

	bool UObject::operator==(const IUValue& other) const
	{
		if (other.getType() == E_UType::Object)
		{
			if (size() != other.getObject().size()){ return false; }
			for (auto it = begin(); it != end(); it++)
			{
				if (it->second != other[it->first])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool UObject::exists(const std::string& key) const
	{
		return m_properties.find(key) != m_properties.end();
	}

	UArray& UObject::getArray()
	{
		return UArrayProvider::getArrayError();
	}

	const UArray& UObject::getArray() const
	{
		return UArrayProvider::getArrayError();
	}

	UValue& UObjectError::operator[](const std::string& key) 
	{
		return UUndefinedProvider::getError();
	}

	UValue& UObjectError::operator[](int index)
	{
		return UUndefinedProvider::getError();		
	}

	UObjectError UObjectProvider::m_error;

	UObject& UObjectProvider::getObjectError()
	{
		return m_error;
	}
}


