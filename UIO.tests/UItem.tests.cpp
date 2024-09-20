#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UItemtests)
	{
		TEST_METHOD(NotFound)
		{
			UObject o{ {"id",1},{"value", 10} };
			Assert::AreEqual(toString(E_UType::Error), toString(o.find("NotFound").getType()));
			Assert::AreEqual(2, (int)o.size());
		}

		TEST_METHOD(Find)
		{
			UObject o{ {"values", UArray{1,UObject{{"item", UObject{{"value", 3}}}}, 3}} };
			Assert::AreEqual(3, o.find("values[1].item.value").getInt());
			Assert::AreEqual(3, o.find("values.2").getInt());
			Assert::AreEqual(-1, o.find("values[3]").getInt(-1));
		}
	};
}