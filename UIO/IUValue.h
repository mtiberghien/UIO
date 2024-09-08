#pragma once
#include "pch.h"
#include "UIO.h"
#include <string>

namespace uio
{
	enum class E_UType { Object, Array, Bool, Short, Int, Float, Double, String, Null, Undefined, Error };
	enum class E_ValueMetaType { Object, Array, StringPrimitive, NonStringPrimitive, Error};

	UIO_API extern std::string toString(E_UType type);
	UIO_API extern E_UType fromString(const std::string& type);

	class UValue;
	class UObject;
	class UArray;

	class UIO_API IUValue
	{
	public:
		IUValue() {}
		virtual ~IUValue() {}
		virtual E_UType getType() const = 0;
		virtual bool getBool(bool defaultValue = false) const = 0;
		virtual short getShort(short defaultValue = 0) const = 0;
		virtual int getInt(int defaultValue = 0) const = 0;
		virtual float getFloat(float defaultValue = 0) const = 0;
		virtual double getDouble(double defaultValue = 0) const = 0;
		virtual std::string getString(const std::string& defaultValue = "") const = 0;
		virtual UObject& getObject() = 0;
		virtual UArray& getArray() = 0;
		virtual UValue& operator[](const std::string& key) = 0;
		virtual UValue& operator[](int index) = 0;
		virtual const UObject& getObject() const = 0;
		virtual const UArray& getArray() const = 0;
		virtual const UValue& operator[](const std::string& key) const = 0;
		virtual const UValue& operator[](int index) const = 0;
		virtual UValue& getOrError(const std::string& key) = 0;
	};
}


