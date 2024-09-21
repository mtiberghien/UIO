#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"
#include "JsonSerializer.h"

using namespace uio;

namespace UIOTests
{

	TEST_CLASS(JsonSerializerTests)
	{
		TEST_METHOD(PropertyWithoutDoubleQuote)
		{
			std::string json = R"({  test:"value", test2  :3})";
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual((size_t)2, o.size());
			Assert::AreEqual(std::string{ "value" }, o["test"].getString());
			Assert::AreEqual(3, o["test2"].getInt());
		}

		TEST_METHOD(SerializeEmptyObject)
		{
			UObject o;
			Assert::AreEqual(std::string{ "{}" }, JsonSerializer::serialize(o));
		}

		TEST_METHOD(DeserializeEmptyArray)
		{
			std::string json = R"({"test":[], "name": "test"})";
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual(2, o.getInt());
			Assert::AreEqual(std::string{ "test" }, o["name"].getString());
			json = "[]";
			UArray a{ 1,2 };
			Assert::IsTrue(JsonSerializer::deserialize(json, a));
			Assert::IsTrue(a.isEmpty());
		}

		TEST_METHOD(DeserializeEmptyObject)
		{
			std::string json = R"({"test":{}, "name": "test"})";
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual(2, o.getInt());
			Assert::AreEqual(std::string{ "test" }, o["name"].getString());
			json = "{}";
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::IsTrue(o.isEmpty());
		}

		TEST_METHOD(UObjectReadError)
		{
			UObject o;

			Assert::IsFalse(JsonSerializer::deserialize("", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"("test":value)", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1")", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":value})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1 "test2":"deux"})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1, "test2": 2,})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":[1,2,3}})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":[1,2 3])", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1,2, 3]})", o));
		}

		TEST_METHOD(UObjectReadString)
		{
			std::string json{ R"({"b": true, "s": 1, "i": 33000, "f": 3.14, "d": 3.141592653, "st": "Test", "n": null, "o": {"s": "Test"}, "a": [1, 2, 3]})" };
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual(9, o.getInt());
			Assert::IsTrue(o["b"].getType() == E_UType::Bool, L"b doit etre booleen");
			Assert::IsTrue(o["b"].getBool(), L"b doit etre vrai");
			Assert::IsTrue(o["s"].getType() == E_UType::Short, L"s doit etre short");
			Assert::AreEqual((short)1, o["s"].getShort());
			Assert::IsTrue(o["i"].getType() == E_UType::Int, L"i doit etre int");
			Assert::AreEqual(33000, o["i"].getInt());
			Assert::IsTrue(o["f"].getType() == E_UType::Double, L"f doit etre double");
			Assert::AreEqual((float)3.14, o["f"].getFloat());
			Assert::IsTrue(o["d"].getType() == E_UType::Double, L"d doit etre double");
			Assert::AreEqual(3.141592653, o["d"].getDouble());
			Assert::IsTrue(o["st"].getType() == E_UType::String, L"st doit etre string");
			Assert::AreEqual(std::string{ "Test" }, o["st"].getString());
			Assert::IsTrue(o["n"].getType() == E_UType::Null, L"n doit etre null");
			Assert::AreEqual(std::string{ "null" }, o["n"].getString());
			Assert::IsTrue(o["o"].getType() == E_UType::Object, L"o doit etre object");
			Assert::AreEqual(1, o["o"].getInt());
			Assert::AreEqual(std::string{ "Test" }, o["o"]["s"].getString());
			Assert::IsTrue(o["a"].getType() == E_UType::Array, L"a doit etre array");
			Assert::AreEqual(3, o["a"].getInt());
			for (int i = 1; i <= 3; i++)
			{
				Assert::AreEqual(i, o["a"][i - 1].getInt());
			}
		}

		TEST_METHOD(UObjectEquality)
		{
			UValue v = UObject{ { "id", 1 }, { "value", "test" } };
			UValue v2 = UObject{ { "id", 1 }, { "value", "test" } };
			UObject o;
			JsonSerializer::deserialize(R"({"id": 1, "value": "test"})", o);
			Assert::IsTrue(v == o);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(UObjectGetString)
		{
			std::string json{ R"({"a": true, "b": 1, "c": 33000, "d": 3.14, "e": 3.141592653, "f": "Test", "g": {"h": "Test"}, "i": [1, 2, 3], "j": null})" };
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual(9, o.getInt());
			Assert::AreEqual(std::string{ "Object{a: Bool, b: Short, c: Int, d: Double, e: Double, f: String, g: Object, i: Array, j: Null}" }, o.getString());
		}

		TEST_METHOD(UObjectWrite)
		{
			std::string json{ R"({"a": true, "b": 1, "c": 33000, "d": 3.14, "e": 3.141592653, "f": "Test", "g": {"h": "Test"}, "i": [1, 2, 3], "j": null})" };
			std::string json2{ R"({
    "a": true, 
    "b": 1, 
    "c": 33000, 
    "d": 3.14, 
    "e": 3.141592653, 
    "f": "Test", 
    "g": 
    {
        "h": "Test"
    }, 
    "i": 
    [
        1, 
        2, 
        3
    ]
}
)" };
			UObject o;
			UObject o2;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::IsTrue(JsonSerializer::deserialize(json2, o2));
			std::ostringstream s;
			std::ostringstream s2;
			JsonSerializer::serialize(s, o, false);
			JsonSerializer::serialize(s2, o2, true);
			Assert::AreEqual(json, s.str());
			Assert::AreEqual(json2, s2.str());
		}

		TEST_METHOD(UObjectFind)
		{
			std::string json{ R"({"b": true, "s": 1, "i": 33000, "f": 3.14, "d": 3.141592653, "st": "Test", "o": {"s": "Test"}, "a": [1, {"value": 2}, 3]})" };
			UObject o;
			JsonSerializer::deserialize(json, o);
			UValue& v = o.find("o.s");
			Assert::IsTrue(v.getType() == E_UType::String);
			Assert::AreEqual(std::string{ "Test" }, v.getString());
			v = o.find("a[1].value");
			Assert::IsTrue(v.getType() == E_UType::Short);
			Assert::AreEqual(2, v.getInt());
			UValue& v2 = o.find("test.not.found");
			Assert::IsTrue(v2.isError());
		}

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

		TEST_METHOD(SerializeArray)
		{
			UArray a;
			std::string json = JsonSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ "[]" }, json);
			a << 1 << 2 << 3;
			json = JsonSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ "[1, 2, 3]" }, json);
		}


	};
}