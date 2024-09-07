#include "pch.h"
#include "UUndefined.h"
#include "UValue.h"
#include "UObject.h"

namespace uio
{
	UValue& UUndefined::operator[](const std::string& key)
	{
		return UUndefinedProvider::getError();
	}

	const UValue& UUndefined::operator[](const std::string& key) const
	{
		return UUndefinedProvider::getError();
	}

	UValue& UUndefined::operator[](int index)
	{
		return UUndefinedProvider::getError();
	}

	UValue& UUndefined::getOrError(const std::string& key)
	{
		return UUndefinedProvider::getError();
	}

	const UValue& UUndefined::operator[](int index) const
	{
		return UUndefinedProvider::getError();
	}

	UObject& UUndefined::getObject()
	{
		return UObjectProvider::getObjectError();
	}

	UArray& UUndefined::getArray()
	{
		return UArrayProvider::getArrayError();
	}

	const UObject& UUndefined::getObject() const
	{
		return UObjectProvider::getObjectError();
	}

	const UArray& UUndefined::getArray() const
	{
		return UArrayProvider::getArrayError();
	}

	UValue UUndefinedProvider::m_void{ E_UType::Error };

	UValue& UUndefinedProvider::getError()
	{
		return m_void;
	}
}
