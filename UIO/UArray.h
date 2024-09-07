#pragma once
#include "UItem.h"
#include "UIO.h"
#include "IUContainer.h"


namespace uio
{
	class UValue;

	template class UIO_API std::vector<UValue>;

	class UIO_API UArray : public UItem, public IUContainer
	{
	public:
		UArray();
		UArray(std::initializer_list<UValue> items) : m_items(items) {}
		virtual ~UArray() { m_items.clear(); }
		bool isEmpty() const override{ return m_items.empty(); }
		E_UType getType() const override { return E_UType::Array; }
		bool getBool(bool defaultValue = false) const override { return !isEmpty(); }
		short getShort(short defaultValue = 0) const override { return (short)m_items.size(); }
		int getInt(int defaultValue = 0) const override { return (int)m_items.size(); }
		float getFloat(float defaultValue = 0) const override { return (float)m_items.size(); }
		double getDouble(double defaultValue = 0) const override { return (double)m_items.size(); }
		UObject& getObject() override;
		UArray& getArray() override { return *this; }
		const UObject& getObject() const override;
		const UArray& getArray() const override { return *this; }
		const UValue& operator[](const std::string& key) const override;
		const UValue& operator[](int index) const override;
		std::string getString(const std::string& defaultValue = "") const override;
		UValue& operator[](const std::string& key) override;
		UValue& operator[](int index) override;
		virtual void push_back(UValue&& item) { m_items.push_back(item); }
		virtual void push_back(const UValue& item) { m_items.push_back(item); }
		virtual UArray& operator<<(const UValue& item) { m_items.push_back(item); return *this; }
		void clear() override { m_items.clear(); }
		std::size_t size() const override { return m_items.size(); }
		std::vector<UValue>::reference back(){ return m_items.back(); }
		std::vector<UValue>::const_reference back() const { return m_items.back(); }
		virtual UArray& operator=(const UArray& array);
		std::vector<UValue>::const_iterator begin() const { return m_items.begin(); }
		std::vector<UValue>::const_iterator end() const { return m_items.end(); }
		std::vector<UValue>::iterator begin() { return m_items.begin(); }
		std::vector<UValue>::iterator end() { return m_items.end(); }
		bool operator==(const IUValue& other) const;
		UValue& getOrError(const std::string& key) override;
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

