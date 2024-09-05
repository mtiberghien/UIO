#pragma once
#include "IUValue.h"
#include "UIO.h"

namespace uio
{
	class UIO_API UItem : public IUValue
	{
	public:
		virtual ~UItem() {}
		virtual bool operator==(const IUValue& other) const = 0;
		bool operator!=(const IUValue& other) const { return !(*this == other); };
		explicit operator bool() const { return getBool(); }
		explicit operator short() const { return getShort(); }
		explicit operator int() const { return getInt(); }
		explicit operator float() const { return getFloat(); }
		explicit operator double() const { return getDouble(); }
		operator std::string() const { return getString(); }
		operator UObject& () { return getObject(); }
		operator UArray& () { return getArray(); }
	    operator const UObject& () const { return getObject(); }
		operator const UArray& () const { return getArray(); }
		bool isError() const { return getType() == E_UType::Error; }
		bool isObject() const { return getType() == E_UType::Object; }
		bool isArray() const { return getType() == E_UType::Array; }
		bool isUndefined() const { return getType() == E_UType::Undefined; }
		bool isNull() const { return getType() == E_UType::Null; }
		bool isPrimitive() const {
			switch (getType())
			{
			case E_UType::Error:
			case E_UType::Object:
			case E_UType::Array:
			case E_UType::Undefined:
				return false;
			default: return true;
			}
		}
		bool isBool() const { return getType() == E_UType::Bool; }
		bool isString() const { return getType() == E_UType::String; }
		UValue& find(const std::string& path);
		const UValue& find(const std::string& path) const;
		bool isNumber() const;

	};

}

