#include "pch.h"
#include "UValue.h"
#include "UObject.h"
#include "UUndefined.h"
#include "UArray.h"
#include "JsonIOHelper.h"

namespace json
{
	UValue::UValue(const UObject& ref) : m_ptr(new UObject(ref))
	{
	}
	UValue::UValue() :m_ptr(new UUndefined()) {}
	UValue::UValue(const UArray& ref) : m_ptr(new UArray(ref))
	{
	}

	UValue::UValue(const UValue& ref)
	{
		switch (ref.getType())
		{
		case E_UType::Error:
			m_ptr = std::make_unique<UUndefined>();
			break;
		case E_UType::Object:
			m_ptr = std::make_unique<UObject>(ref.getObject());
			break;
		case E_UType::Array:
			m_ptr = std::make_unique <UArray>(ref.getArray());
			break;
		case E_UType::Bool:
		{
			m_ptr = std::make_unique<UPrimitive<bool>>(ref.getBool());
			break;
		}
		case E_UType::Short:
		{
			m_ptr = std::make_unique<UPrimitive<short>>(ref.getShort());
			break;
		}
		case E_UType::Int:
		{
			m_ptr = std::make_unique<UPrimitive<int>>(ref.getInt());
			break;
		}
		case E_UType::Float:
		{
			m_ptr = std::make_unique<UPrimitive<float>>(ref.getFloat());
			break;
		}
		case E_UType::Double:
		{
			m_ptr = std::make_unique<UPrimitive<double>>(ref.getDouble());
			break;
		}
		case E_UType::String:
		{
			m_ptr = std::make_unique<UPrimitive<std::string>>(ref.getString());
			break;
		}
		case E_UType::Null:
		{
			m_ptr = std::make_unique<UPrimitive<std::nullptr_t>>(nullptr);
			break;
		}
		case E_UType::Undefined:
		{
			m_ptr = std::make_unique<UUndefined>();
			break;
		}
		default:
			m_ptr = std::make_unique<JUndefinedError>();
			break;
		}
	}

	UValue::UValue(E_UType a_Type)
	{
		switch (a_Type)
		{
		case E_UType::Bool:
		{
			m_ptr = std::make_unique<UPrimitive<bool>>(false);
			break;
		}
		case E_UType::Short:
		{
			m_ptr = std::make_unique<UPrimitive<short>>((short)0);
			break;
		}
		case E_UType::Int:
		{
			m_ptr = std::make_unique<UPrimitive<int>>(0);
			break;
		}
		case E_UType::Float:
		{
			m_ptr = std::make_unique<UPrimitive<float>>((float)0);
			break;
		}
		case E_UType::Double:
		{
			m_ptr = std::make_unique<UPrimitive<double>>((double)0);
			break;
		}
		case E_UType::String:
		{
			m_ptr = std::make_unique<UPrimitive<std::string>>("");
			break;
		}
		case E_UType::Object:
		{
			m_ptr = std::make_unique<UObject>();
			break;
		}
		case E_UType::Array:
			m_ptr = std::make_unique<UArray>();
			break;
		case E_UType::Null:
		{
			m_ptr = std::make_unique<UPrimitive<std::nullptr_t>>(nullptr);
			break;
		}
		case E_UType::Undefined:
		{
			m_ptr = std::make_unique<UUndefined>();
			break;
		}
		default:
			m_ptr = std::make_unique<JUndefinedError>();
			break;
		}
	}

	bool UValue::operator==(const IUValue& other) const
	{
		if (isObject())
		{
			return getObject() == other.getObject();
		}
		else if (isArray())
		{
			return getArray() == other.getArray();
		}
		else
		{
			return getString() == getString();
		}
	}
}


