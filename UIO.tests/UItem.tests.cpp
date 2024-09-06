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
	};
}