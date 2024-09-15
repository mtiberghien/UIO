#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"
#include "JsonSerializer.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UArraytests)
	{
		TEST_METHOD(UArrayStringInit)
		{
			UArray a;
			JsonSerializer::deserialize(R"([1, "2", {"id": "test"}, [null, 1]])", a);
			UValue v = UObject{ { "id", "test" } };
			Assert::AreEqual((size_t)4, a.size());
			Assert::AreEqual(1, (int)a[0]);
			Assert::AreEqual(std::string{ "2" }, a[1].getString());
			Assert::IsTrue(a[2] == v);
			Assert::IsTrue(a[3][0].isNull());
			Assert::AreEqual(1, a.find("[3][1]").getInt());
		}

		TEST_METHOD(UArrayGetString)
		{
			std::string json{ R"([1, "2", {"id": "test"}, [null, 1]])" };
			UArray a;
			Assert::IsTrue(JsonSerializer::deserialize(json, a));
			Assert::AreEqual(4, a.getInt());
			Assert::AreEqual(std::string{ "Array[4]" }, a.getString());
		}

		TEST_METHOD(UArrayEquality)
		{
			UObject o{ { "id", 1 }, { "value", "test" } };
			UValue v = UArray{ 1, "deux", nullptr, o };
			UValue v2{ E_UType::Array };
			JsonSerializer::deserialize(R"([1, "deux", null, {"id": 1, "value": "test"}])", v2.getArray());
			UArray a;
			a << 1 << "deux" << nullptr << UObject{ { "id", 1 }, { "value", "test" } };
			Assert::IsTrue(v == a);
			Assert::IsTrue(v == v2);
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