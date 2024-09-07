#include "pch.h"
#include "UArray.h"
#include "UValue.h"
#include "UIOHelper.h"
#include "UObject.h"


namespace uio
{
	UArray::UArray() {};
	UValue& UArray::operator[](const std::string& key) {
		double d;
		E_UType t;
		if (UIOHelper::tryGetNumber(key, d, t))
		{
			return m_items[(int)d];
		}
		return UUndefinedProvider::getError();
	}

	UValue& UArray::getOrError(const std::string& key) {
		return this->operator[](key);
	}

	const UValue& UArray::operator[](const std::string& key) const {
		double d;
		E_UType t;
		if (UIOHelper::tryGetNumber(key, d, t))
		{
			return m_items.at((int)d);
		}
		return UUndefinedProvider::getError();
	}

	UValue& UArray::operator[](int index)
	{
		if (index >= 0 && (unsigned int)index < m_items.size())
		{
			return m_items[index];
		}
		return UUndefinedProvider::getError();
	}

	const UValue& UArray::operator[](int index) const
	{
		if (index >= 0 && (unsigned int)index < m_items.size())
		{
			return m_items.at(index);
		}
		return UUndefinedProvider::getError();
	}

	UArray& UArray::operator=(const UArray& array)
	{
		clear();
		for (const auto& it : array.m_items)
		{
			m_items.push_back(it);
		}
		return *this;
	}

	std::string UArray::getString(const std::string& defaultValue) const
	{
		std::ostringstream s;
		s << "Array[" << size() << "]";
		return s.str();
	}

	bool UArray::operator==(const IUValue& other) const
	{
		if (other.getType() == E_UType::Array)
		{
			if (size() != other.getArray().size()){ return false; }
			for (int i = 0; (size_t)i < size(); i++)
			{
				if (this->operator [](i) != other[i])
				{
					return false;
				}
			}
			return true;
		}

		return false;
	}


	UObject& UArray::getObject()
	{
		return UObjectProvider::getObjectError();
	}

	const UObject& UArray::getObject() const
	{
		return UObjectProvider::getObjectError();
	}

	UArrayError UArrayProvider::m_error;

	UArray& UArrayProvider::getArrayError()
	{
		return m_error;
	}
}

