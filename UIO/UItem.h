#pragma once
#include "IUValue.h"
#include "UIO.h"

namespace uio
{
	class UItem : public IUValue
	{
	public:
		virtual ~UItem() {}
		UIO_API virtual bool operator==(const IUValue& other) const = 0;
		UIO_API bool operator!=(const IUValue& other) const { return !(*this == other); };
		UIO_API explicit operator bool() const { return getBool(); }
		UIO_API explicit operator short() const { return getShort(); }
		UIO_API explicit operator int() const { return getInt(); }
		UIO_API explicit operator float() const { return getFloat(); }
		UIO_API explicit operator double() const { return getDouble(); }
		UIO_API operator std::string() const { return getString(); }
		UIO_API operator UObject& () { return getObject(); }
		UIO_API operator UArray& () { return getArray(); }
		UIO_API operator const UObject& () const { return getObject(); }
		UIO_API operator const UArray& () const { return getArray(); }
		UIO_API bool isError() const { return getType() == E_UType::Error; }
		UIO_API bool isObject() const { return getType() == E_UType::Object; }
		UIO_API bool isArray() const { return getType() == E_UType::Array; }
		UIO_API bool isUndefined() const { return getType() == E_UType::Undefined; }
		UIO_API bool isNull() const { return getType() == E_UType::Null; }
		UIO_API bool isPrimitive() const {
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
		UIO_API bool isBool() const { return getType() == E_UType::Bool; }
		UIO_API bool isString() const { return getType() == E_UType::String; }
		UIO_API UValue& find(const std::string& path);
		UIO_API const UValue& find(const std::string& path) const;
		UIO_API bool isNumber() const;

	};

}

