#pragma once
#include "IUValue.h"
#include <map>
#include <string>
#include "UIO.h"
#include "UIOHelper.h"


namespace uio
{

	class JsonIOHelper
	{
	public:

		static E_ValueMetaType readValueType(std::istream& stream);

		static bool readStringValue(std::istream& stream, std::string& value);

		static bool  isNumber(const std::string& value);

		static bool setNonStringPrimitiveValue(std::string& primitive, double& result, E_UType& type);

		static bool readNonStringPrimitive(std::istream& stream, double& result, E_UType& type);

	};

	
}


