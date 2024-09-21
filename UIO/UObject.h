#pragma once
#include "UItem.h"
#include "UIO.h"
#include "IUContainer.h"
#include <map>
#include <string>
#include <initializer_list>

namespace uio
{
	class UValue;

	class UObject : public UItem, public IUContainer
	{
	public:
		UIO_API UObject() {}
		UIO_API UObject(const std::string& name) : m_class(name) {}
		UIO_API UObject(const std::string& name, std::initializer_list < std::pair<const std::string, UValue>> properties);
		UIO_API UObject(std::initializer_list < std::pair<const std::string, UValue>> properties) : UObject("", properties) {}
		UIO_API virtual ~UObject() { m_properties.clear(); }
		UIO_API bool isEmpty() const override { return m_properties.empty(); }
		UIO_API E_UType getType() const override { return E_UType::Object; }
		UIO_API bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		UIO_API short getShort(short defaultValue = 0) const override { return (short)m_properties.size(); }
		UIO_API int getInt(int defaultValue = 0) const override { return (int)m_properties.size(); }
		UIO_API float getFloat(float defaultValue = 0) const override { return (float)m_properties.size(); }
		UIO_API double getDouble(double defaultValue = 0) const override { return (double)m_properties.size(); }
		UIO_API std::string getString(const std::string& defaultValue = "") const override;
		UIO_API UObject& getObject() override { return *this; }
		UIO_API UArray& getArray() override;
		UIO_API const UObject& getObject() const override { return *this; };
		UIO_API const UArray& getArray() const override;
		UIO_API virtual UValue& operator[](const std::string& key) override;
		UIO_API const UValue& operator[](const std::string& key) const override;
		UIO_API const UValue& operator[](int index) const override;
		UIO_API virtual UValue& operator[](int index) override;
		UIO_API std::vector<std::string> getKeys() const;
		UIO_API void clear() override { m_properties.clear(); }
		UIO_API bool exists(const std::string& key) const;
		UIO_API size_t size() const override { return (int)m_properties.size(); }
		UIO_API virtual UObject& operator=(const UObject& object);
		UIO_API std::map<std::string, UValue>::const_iterator begin() const { return m_properties.begin(); }
		UIO_API std::map<std::string, UValue>::const_iterator end() const { return m_properties.end(); }
		UIO_API std::map<std::string, UValue>::iterator begin() { return m_properties.begin(); }
		UIO_API std::map<std::string, UValue>::iterator end() { return m_properties.end(); }
		UIO_API bool operator==(const IUValue& other) const override;
		UIO_API UValue& getOrError(const std::string& key) override;
		UIO_API const std::string& getClass() const { return m_class; }
		UIO_API virtual void setClass(const std::string& name) { m_class = name; }
		UIO_API bool erase(const std::string& key) override;
		UIO_API bool eraseAt(int index) override;

	private:
		std::string m_class{ "" };
		std::map<std::string, UValue> m_properties;
	};

	class UObjectError : public UObject
	{
	public:
		UObjectError() {}
		UObjectError(const UObject&& object) {}
		E_UType getType() const override { return E_UType::Error; }
		UValue& operator[](const std::string& key) override;
		UValue& operator[](int index) override;
		UObject& operator=(const UObject& object) override { return *this; }
		void setClass(const std::string& name) override {}
	};

	class UObjectProvider
	{
	public:
		static UIO_API UObject& getObjectError();
	private:
		static UObjectError m_error;
	};
}


