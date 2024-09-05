#pragma once
#include "UItem.h"
#include "UPrimitive.h"
#include "UIO.h"

namespace uio
{
	class UObject;
	class JUndefined;
	class UArray;

	template class UIO_API std::unique_ptr<IUValue>;

	class UIO_API UValue : public UItem
	{
	public:
		UValue();
		UValue(E_UType a_Type);
		template<class T>
		UValue(T value) :m_ptr(std::make_unique<UPrimitive<T>>(value)) {
		}
		UValue(const UObject& ref);
		UValue(const UArray& ref);
		UValue(const UValue& ref);
		E_UType getType() const override { return m_ptr->getType(); }
		bool getBool(bool defaultValue = false) const override { return m_ptr->getBool(defaultValue); }
		short getShort(short defaultValue = 0) const override { return m_ptr->getShort(defaultValue); }
		int getInt(int defaultValue = 0) const override { return m_ptr->getInt(defaultValue); }
		float getFloat(float defaultValue = 0) const override { return m_ptr->getFloat(defaultValue); }
		double getDouble(double defaultValue = 0) const override { return m_ptr->getDouble(defaultValue); }
		std::string getString(const std::string& defaultValue = "") const override { return m_ptr->getString(defaultValue); }
		UObject& getObject() override { return m_ptr->getObject(); }
		UArray& getArray() override { return m_ptr->getArray(); }
		const UObject& getObject() const override { return m_ptr->getObject(); }
		const UArray& getArray() const override { return m_ptr->getArray(); }
		const UValue& operator[](const std::string& key) const override { return m_ptr->operator[](key); }
		const UValue& operator[](int index) const override { return m_ptr->operator[](index); }
		UValue& operator[](const std::string& key) override { return m_ptr->operator[](key); }
		UValue& operator[](int index) override { return m_ptr->operator[](index); }
		UValue& operator=(const UValue& value)
		{
			if (getType() != E_UType::Error)
			{
				this->m_ptr = UValue(value).m_ptr;
			}
			return *this;
		}
		bool operator==(const IUValue& other) const override;
	private:
		std::unique_ptr<IUValue> m_ptr;

	};
}


