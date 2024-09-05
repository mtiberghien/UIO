#pragma once
#include "UItem.h"
#include "JsonIO.h"
#include <vector>
#include "IUContainer.h"

namespace json
{
	class UValue;

	template class JSONIO_API std::map<std::string, UValue>;

	class JSONIO_API UObject : public UItem, public IUContainer
	{
	public:
		UObject() {}
		UObject(std::initializer_list < std::pair<const std::string, UValue>> properties);
		virtual ~UObject() { m_properties.clear(); }
		bool isEmpty() const override { return m_properties.empty(); }
		E_UType getType() const override { return E_UType::Object; }
		bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		short getShort(short defaultValue = 0) const override { return (short)m_properties.size(); }
		int getInt(int defaultValue = 0) const override { return (int)m_properties.size(); }
		float getFloat(float defaultValue = 0) const override { return (float)m_properties.size(); }
		double getDouble(double defaultValue = 0) const override { return (double)m_properties.size(); }
		std::string getString(const std::string& defaultValue = "") const override;
		UObject& getObject() override { return *this; }
		UArray& getArray() override;
		const UObject& getObject() const override { return *this; };
		const UArray& getArray() const override; 
		virtual UValue& operator[](const std::string& key) override;
		const UValue& operator[](const std::string& key) const override;
		const UValue& operator[](int index) const override;
		virtual UValue& operator[](int index) override;
		std::vector<std::string> getKeys() const;
		void clear() override { m_properties.clear(); }
		bool exists(const std::string& key) const;
		size_t size() const override { return (int)m_properties.size(); }
		virtual UObject& operator=(const UObject& object);
		std::map<std::string, UValue>::const_iterator begin() const { return m_properties.begin(); }
		std::map<std::string, UValue>::const_iterator end() const { return m_properties.end(); }
		bool operator==(const IUValue& other) const override;
	private:
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
	};

	class UObjectProvider
	{
	public:
		static JSONIO_API UObject& getObjectError();
	private:
		static UObjectError m_error;
	};
}


