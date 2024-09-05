#include "pch.h"
#include "UUndefined.h"
#include "UValue.h"
#include "UObject.h"

namespace uio
{
	UValue& UUndefined::operator[](const std::string& key)
	{
		return JVoidProvider::getError();
	}

	const UValue& UUndefined::operator[](const std::string& key) const
	{
		return JVoidProvider::getError();
	}

	UValue& UUndefined::operator[](int index)
	{
		return JVoidProvider::getError();
	}

	const UValue& UUndefined::operator[](int index) const
	{
		return JVoidProvider::getError();
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

	UValue JVoidProvider::m_void{ E_UType::Error };

	UValue& JVoidProvider::getError()
	{
		return m_void;
	}
}
