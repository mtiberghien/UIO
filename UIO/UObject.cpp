#include "pch.h"
#include "UObject.h"
#include "UValue.h"
#include "UArray.h"

namespace uio
{
	UObject::UObject(std::initializer_list < std::pair<const std::string, UValue>> properties): m_properties(properties)
	{
	}

	UObject& UObject::operator=(const UObject& object)
	{
		clear();
		for (const auto& it : object.m_properties)
		{
			this->operator [](it.first) = it.second;
		}
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
			return JVoidProvider::getError();
		}
		return m_properties.at(key);
	}

	UValue& UObject::operator[](int index)
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return it->second;
		}
		return JVoidProvider::getError();
	}

	const UValue& UObject::operator[](int index) const
	{
		if (index >= 0 && (unsigned int)index < m_properties.size())
		{
			auto it = m_properties.begin();
			std::advance(it, index);
			return m_properties.at(it->first);
		}
		return JVoidProvider::getError();
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
		s << "Object{";
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
		return JVoidProvider::getError();
	}

	UValue& UObjectError::operator[](int index)
	{
		return JVoidProvider::getError();		
	}

	UObjectError UObjectProvider::m_error;

	UObject& UObjectProvider::getObjectError()
	{
		return m_error;
	}
}


