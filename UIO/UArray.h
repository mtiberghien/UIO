#pragma once
#include "UItem.h"
#include "UIO.h"
#include "IUContainer.h"
#include <vector>
#include <string>

namespace uio
{
	class UValue;


	class UArray : public UItem, public IUContainer
	{
	public:
		UIO_API UArray();
		UIO_API UArray(std::initializer_list<UValue> items) : m_items(items) {}
		UIO_API virtual ~UArray() { m_items.clear(); }
		UIO_API bool isEmpty() const override{ return m_items.empty(); }
		UIO_API E_UType getType() const override { return E_UType::Array; }
		UIO_API bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		UIO_API short getShort(short defaultValue = 0) const override { return (short)m_items.size(); }
		UIO_API int getInt(int defaultValue = 0) const override { return (int)m_items.size(); }
		UIO_API float getFloat(float defaultValue = 0) const override { return (float)m_items.size(); }
		UIO_API double getDouble(double defaultValue = 0) const override { return (double)m_items.size(); }
		UIO_API UObject& getObject() override;
		UIO_API UArray& getArray() override { return *this; }
		UIO_API const UObject& getObject() const override;
		UIO_API const UArray& getArray() const override { return *this; }
		UIO_API const UValue& operator[](const std::string& key) const override;
		UIO_API const UValue& operator[](int index) const override;
		UIO_API std::string getString(const std::string& defaultValue = "") const override;
		UIO_API UValue& operator[](const std::string& key) override;
		UIO_API UValue& operator[](int index) override;
		UIO_API virtual void push_back(UValue&& item) { m_items.push_back(item); }
		UIO_API virtual void push_back(const UValue& item) { m_items.push_back(item); }
		UIO_API virtual UArray& operator<<(const UValue& item) { m_items.push_back(item); return *this; }
		UIO_API void clear() override { m_items.clear(); }
		UIO_API std::size_t size() const override { return m_items.size(); }
		UIO_API std::vector<UValue>::reference back(){ return m_items.back(); }
		UIO_API std::vector<UValue>::const_reference back() const { return m_items.back(); }
		UIO_API virtual UArray& operator=(const UArray& array);
		UIO_API std::vector<UValue>::const_iterator begin() const { return m_items.begin(); }
		UIO_API std::vector<UValue>::const_iterator end() const { return m_items.end(); }
		UIO_API std::vector<UValue>::iterator begin() { return m_items.begin(); }
		UIO_API std::vector<UValue>::iterator end() { return m_items.end(); }
		UIO_API bool operator==(const IUValue& other) const;
		UIO_API UValue& getOrError(const std::string& key) override;
		UIO_API bool eraseAt(int index) override;
		UIO_API bool erase(const std::string& key) override;
	private:
		std::vector<UValue> m_items;
	};

	class UArrayError : public UArray
	{
		E_UType getType() const override { return E_UType::Error; }
		void push_back(UValue&& item) override {}
		void push_back(const UValue& item) override {}
		UArray& operator<<(const UValue& item) override { return *this; }
		UArray& operator=(const UArray& array) override { return *this; }
	};

	class UArrayProvider
	{
	public:
		static UIO_API UArray& getArrayError();
	private:
		static UArrayError m_error;
	};
}

