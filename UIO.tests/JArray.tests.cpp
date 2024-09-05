#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Json.h"

using namespace json;

namespace JsonIOtests
{
	TEST_CLASS(JsonArraytests)
	{
		TEST_METHOD(JArrayStringInit)
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

		TEST_METHOD(JArrayGetString)
		{
			std::string json{ R"([1, "2", {"id": "test"}, [null, 1]])" };
			UArray a;
			Assert::IsTrue(JsonSerializer::deserialize(json, a));
			Assert::AreEqual(4, a.getInt());
			Assert::AreEqual(std::string{ "Array[4]" }, a.getString());
		}

		TEST_METHOD(JArrayEquality)
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
	};
}