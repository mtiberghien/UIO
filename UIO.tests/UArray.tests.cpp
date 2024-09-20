#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UArraytests)
	{

		TEST_METHOD(GetString)
		{
			UArray a{ 1,2,3 };
			Assert::AreEqual(std::string{ "Array[3]" }, a.getString());
			a.eraseAt(0);
			Assert::AreEqual(std::string{ "Array[2]" }, a.getString());
			a << 4 << 5 << 6;
			Assert::AreEqual(std::string{ "Array[5]" }, a.getString());
		}
		TEST_METHOD(AffectsFromInnerElement)
		{
			UArray a{ 1,UArray{1,2,3}, 2 };
			UArray aRef = a;
			aRef = aRef[1];
			Assert::AreEqual(3, (int)aRef.size());
			Assert::AreEqual(1, aRef[0].getInt());
			Assert::AreEqual(3, aRef[2].getInt());
		}

		TEST_METHOD(Erase)
		{
			UArray a{ 1,2,3,4 };
			Assert::IsTrue(a.eraseAt(3));
			Assert::AreEqual(3, (int)a.size());
			Assert::IsFalse(a.eraseAt(3));
			Assert::IsTrue(a.erase("1"));
			Assert::AreEqual(2, (int)a.size());
			Assert::IsFalse(a.erase("test"));
			Assert::IsFalse(a.erase("4"));
			Assert::AreEqual(1, a[0].getInt());
			Assert::AreEqual(3, a[1].getInt());
		}
	};
}