#include "pch.h"
#include "UItem.h"
#include "UIOHelper.h"
#include "UUndefined.h"
#include "UValue.h"

namespace uio
{
	std::string toString(E_UType type)
	{
		switch (type)
		{
		case E_UType::Object: return "Object";
		case E_UType::Array: return "Array";
		case E_UType::Bool: return "Bool";
		case E_UType::Short: return "Short";
		case E_UType::Int: return "Int";
		case E_UType::Float: return "Float";
		case E_UType::Double: return "Double";
		case E_UType::String: return "String";
		case E_UType::Null: return "Null";
		case E_UType::Error: return "Error";
		default: return "Undefined";
		}
	}

	bool UItem::isNumber() const
	{
		if (isPrimitive() && !isNull())
		{
			if (!isString())
			{
				return true;
			}
			else
			{
				double d;
				E_UType t;
				return UIOHelper::tryGetNumber(getString(), d, t);
			}
		}
		return false;
	}

	bool findFieldPart(std::istream& stream, std::string& fieldPart)
	{
		std::ostringstream s;
		int sizeRead = 0;
		while (!stream.eof())
		{
			char c = stream.get();
			bool terminationChar = c == '[' || c == ']' || c == '.';
			if (terminationChar && sizeRead > 0)
			{
				break;
			}
			if (!terminationChar && !isspace(c) && c!=EOF)
			{
				s << c;
				sizeRead++;
			}
		}
		fieldPart = s.str();
		return sizeRead > 0;
	}

	UValue& getValueRecursive(UValue& result, std::istream& stream)
	{
		if (!result.isError())
		{
			std::string fieldPart;
			if (findFieldPart(stream, fieldPart))
			{
				return getValueRecursive(result.getIfExists(fieldPart), stream);
			}
		}

		return result;
	}

	const UValue& getValueRecursive(const UValue& result, std::istream& stream)
	{
		if (!result.isError())
		{
			std::string fieldPart;
			if (findFieldPart(stream, fieldPart))
			{
				return getValueRecursive(result[fieldPart], stream);
			}
		}

		return result;
	}


	UValue& UItem::find(const std::string& path)
	{
		std::string fieldPart;
		std::istringstream ps{ path };
		if (findFieldPart(ps, fieldPart))
		{
			return getValueRecursive(this->getIfExists(fieldPart), ps);
		}

		return UUndefinedProvider::getError();
	}

	const UValue& UItem::find(const std::string& path) const
	{
		std::string fieldPart;
		std::istringstream ps{ path };
		if (findFieldPart(ps, fieldPart))
		{
			return getValueRecursive(this->operator[](fieldPart), ps);
		}

		return UUndefinedProvider::getError();
	}
}

