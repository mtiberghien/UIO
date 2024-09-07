#pragma once
#include "UItem.h"
#include "UIO.h"


namespace uio
{
	class UUndefined : public IUValue
	{
	public:
		UUndefined() {}
		~UUndefined() {}
		E_UType getType() const override { return E_UType::Undefined; }
		bool getBool(bool defaultValue = false) const override { return defaultValue; }
		short getShort(short defaultValue = 0) const override { return defaultValue; }
		int getInt(int defaultValue = 0) const override { return defaultValue; }
		float getFloat(float defaultValue = 0) const override { return defaultValue; }
		double getDouble(double defaultValue = 0) const override { return defaultValue; }
		std::string getString(const std::string& defaultValue = "") const override { return defaultValue; }
		UObject& getObject() override;
		UArray& getArray() override;
		const UObject& getObject() const override;
		const UArray& getArray() const override;
		UValue& operator[](const std::string& key) override;
		const UValue& operator[](const std::string& key) const override;
		UValue& operator[](int index) override;
		const UValue& operator[](int index) const override;
		UValue& getOrError(const std::string& key) override;
	};

	class UUndefinedError : public UUndefined
	{
	public :
		UUndefinedError() {}
		E_UType getType() const override { return E_UType::Error; }
	};

	class UUndefinedProvider
	{
	public:
		static UIO_API UValue& getError();
	private:
		static UValue m_void;
	};
}


