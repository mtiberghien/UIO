#pragma once
#include "UItem.h"
#include "UPrimitive.h"
#include "UIO.h"

namespace uio
{
	class UObject;
	class JUndefined;
	class UArray;

	class UValue : public UItem
	{
	public:
		UIO_API UValue();
		UIO_API UValue(E_UType a_Type);
		template<class T>
		UValue(T value) :m_ptr(std::make_unique<UPrimitive<T>>(value)) {
		}
		UIO_API UValue(const UObject& ref);
		UIO_API UValue(const UArray& ref);
		UIO_API UValue(const UValue& ref);
		UIO_API E_UType getType() const override { return m_ptr->getType(); }
		UIO_API bool getBool(bool defaultValue = false) const override { return m_ptr->getBool(defaultValue); }
		UIO_API short getShort(short defaultValue = 0) const override { return m_ptr->getShort(defaultValue); }
		UIO_API int getInt(int defaultValue = 0) const override { return m_ptr->getInt(defaultValue); }
		UIO_API float getFloat(float defaultValue = 0) const override { return m_ptr->getFloat(defaultValue); }
		UIO_API double getDouble(double defaultValue = 0) const override { return m_ptr->getDouble(defaultValue); }
		UIO_API std::string getString(const std::string& defaultValue = "") const override { return m_ptr->getString(defaultValue); }
		UIO_API UObject& getObject() override { return m_ptr->getObject(); }
		UIO_API UArray& getArray() override { return m_ptr->getArray(); }
		UIO_API const UObject& getObject() const override { return m_ptr->getObject(); }
		UIO_API const UArray& getArray() const override { return m_ptr->getArray(); }
		UIO_API const UValue& operator[](const std::string& key) const override { return m_ptr->operator[](key); }
		UIO_API const UValue& operator[](int index) const override { return m_ptr->operator[](index); }
		UIO_API UValue& operator[](const std::string& key) override { return m_ptr->operator[](key); }
		UIO_API UValue& operator[](int index) override { return m_ptr->operator[](index); }
		UIO_API UValue& getOrError(const std::string& key) override { return m_ptr->getOrError(key); }
		UIO_API UValue& operator=(const UValue& value)
		{
			if (getType() != E_UType::Error)
			{
				this->m_ptr = UValue(value).m_ptr;
			}
			return *this;
		}
		UIO_API bool operator==(const IUValue& other) const override;
	private:
		std::unique_ptr<IUValue> m_ptr;

	};
}


