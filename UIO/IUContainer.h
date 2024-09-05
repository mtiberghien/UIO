#pragma once
#include "UIO.h"

namespace uio
{
	class UIO_API IUContainer
	{
	public:
		virtual ~IUContainer() {}
		virtual std::size_t size() const = 0;
		virtual void clear() = 0;
		virtual bool isEmpty() const = 0;
	};
}


