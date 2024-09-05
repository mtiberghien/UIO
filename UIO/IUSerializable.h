#pragma once
#include "UIO.h"
#include "UObject.h"

namespace uio
{
	class UIO_API IUSerializable
	{
	public:
		IUSerializable() {};
		virtual ~IUSerializable() {};
		virtual void toObject(UObject& object) const = 0;
		virtual void fromObject(const UObject& object) = 0;

	};

}


