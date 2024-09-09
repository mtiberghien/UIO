#pragma once
#include "UItem.h"
#include "UUndefined.h"
#include "UIOHelper.h"
#include "UIO.h"
#include "UObject.h"
#include "UArray.h"

namespace uio
{
	class UUndefined;
	class UValue;


	template<class T>
	class UPrimitive : public IUValue
	{
	public:
		UPrimitive(const T& value) : m_value(value)
		{
			static_assert(std::is_fundamental<T>::value ||
				std::is_base_of<std::string, T>::value ||
				std::is_same<const char*, T>::value ||
				std::is_base_of<nullptr_t, T>::value, "Value should be a c++ primitive or std::string or un const char* or un nullptr_t");
		}
		UPrimitive(T&& value) : m_value(std::move(value)) {}
		~UPrimitive()
		{
		}
		E_UType getType() const override { return getType<T>(); }
		bool getBool(bool defaultValue = false) const override
		{
			return getBool<T>(defaultValue);
		}
		short getShort(short defaultValue = 0) const override 
		{ 
			return getShort<T>(defaultValue);
		}
		int getInt(int defaultValue = 0) const override 
		{ 
			return getInt<T>(defaultValue); 
		}
		float getFloat(float defaultValue = 0) const override
		{
			return getFloat<T>(defaultValue);
		}
		double getDouble(double defaultValue = 0) const override
		{
			return getDouble<T>(defaultValue);
		}

		std::string getString(const std::string& defaultValue = "") const override
		{
			return getString<T>(defaultValue);
		}

		UValue& operator[](const std::string& key) override
		{
			return UUndefinedProvider::getError();
		}

		const UValue& operator[](const std::string& key) const override
		{
			return UUndefinedProvider::getError();
		}

		UValue& operator[](int index) override
		{
			return UUndefinedProvider::getError();
		}

		const UValue& operator[](int index) const override
		{
			return UUndefinedProvider::getError();
		}

		UObject& getObject() override { return UObjectProvider::getObjectError(); }
		UArray& getArray() override { return UArrayProvider::getArrayError(); }
		const UObject& getObject() const override { return UObjectProvider::getObjectError(); }
		const UArray& getArray() const override { return UArrayProvider::getArrayError(); }
		UValue& getOrError(const std::string& key) override { return UUndefinedProvider::getError(); }

	private:
		template<class T>
		E_UType getType() const
		{
			switch (sizeof(T))
			{
				case sizeof(bool) : return E_UType::Bool;
				case sizeof(short) : return E_UType::Short;
				case sizeof(int) : return E_UType::Int;
				case sizeof(double) : return E_UType::Double;
				default: return E_UType::Undefined;
			}
		}
		template<>
		E_UType getType<float>() const { return E_UType::Float; }
		template<>
		E_UType getType<std::string>() const { return E_UType::String; }
		template<>
		E_UType getType<const char*>() const { return E_UType::String; }
		template<>
		E_UType getType<std::nullptr_t>() const { return E_UType::Null; }
		template<class T>
		std::string getString(const std::string& defaultValue = "") const
		{
			std::ostringstream s;
			s << m_value;
			return s.str();
		}
		template<>
		std::string getString<bool>(const std::string& defaultValue) const
		{
			return getBool() ? "true" : "false";
		}
		template<>
		std::string getString<std::nullptr_t>(const std::string& defaultValue) const
		{
			return "null";
		}
		template<>
		std::string getString<double>(const std::string& defaultValue) const
		{
			std::ostringstream s;
			s << std::setprecision(std::numeric_limits<double>::digits10 + 1);
			s << m_value;
			return s.str();
		}
		template<class T>
		float getFloat(float defaultValue) const
		{
			return (float)m_value;

		}
		template<>
		float getFloat<const char*>(float defaultValue) const
		{
			double d;
			E_UType t;
			if (UIOHelper::tryGetNumber(getString(), d, t))
			{
				return (float)d;
			}
			return defaultValue;
		}
		template<>
		float getFloat<std::string>(float defaultValue) const
		{
			return getFloat<const char*>(defaultValue);
		}
		template<>
		float getFloat<std::nullptr_t>(float defaultValue) const
		{
			return getFloat<const char*>(defaultValue);
		}
		
		template<class T>
		double getDouble(double defaultValue) const
		{
			return (double)m_value;
		}
		template<>
		double getDouble<const char*>(double defaultValue) const
		{
			double d;
			E_UType t;
			if (UIOHelper::tryGetNumber(this->getString(), d, t))
			{
				return d;
			}
			return defaultValue;
		}
		template<>
		double getDouble<std::string>(double defaultValue) const
		{
			return getDouble<const char*>(defaultValue);
		}
		template<>
		double getDouble<std::nullptr_t>(double defaultValue) const
		{
			return getDouble<const char*>(defaultValue);
		}
		template<class T>
		int getInt(int defaultValue) const
		{
			return (int)m_value;
		}
		template<>
		int getInt<const char*>(int defaultValue) const
		{
			double d;
			E_UType t;
			if (UIOHelper::tryGetNumber(getString(), d, t))
			{
				return (int)d;
			}
			return defaultValue;
		}
		template<>
		int getInt<std::string>(int defaultValue) const
		{
			return getInt<const char*>(defaultValue);
		}
		template<>
		int getInt<std::nullptr_t>(int defaultValue) const
		{
			return getInt<const char*>(defaultValue);
		}
		template<class T>
		short getShort(short defaultValue) const
		{
			return (short)m_value;
		}
		template<>
		short getShort<const char*>(short defaultValue) const
		{
			double d;
			E_UType t;
			if (UIOHelper::tryGetNumber(getString(), d, t))
			{
				return (short)d;
			}
			return defaultValue;
		}
		template<>
		short getShort<std::string>(short defaultValue) const
		{
			return getShort<const char*>(defaultValue);
		}
		template<>
		short getShort<std::nullptr_t>(short defaultValue) const
		{
			return getShort<const char*>(defaultValue);
		}
		template<class T>
		bool getBool(bool defaultValue) const
		{
			return m_value == 1;
		}
		template<>
		bool getBool<const char*>(bool defaultValue) const
		{
			return !getString().empty();
		}
		template<>
		bool getBool<std::string>(bool defaultValue) const
		{
			return getBool<const char*>(defaultValue);
		}
		template<>
		bool getBool<std::nullptr_t>(bool defaultValue) const
		{
			return false;
		}

		T m_value;
	};

}

