#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"
#include "JsonSerializer.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UObjecttests)
	{
	public:
		
		TEST_METHOD(JObjectIsEmpty)
		{
			UObject o;
			Assert::IsTrue(o.isEmpty(), L"o doit etre vide");
		}

		TEST_METHOD(JObjectAddBool)
		{
			UObject o;
			o["b"] = true;
			o["b2"] = false;
			Assert::AreEqual(2, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue( o["b"].getType() == E_UType::Bool);
			Assert::IsTrue(o["b2"].isBool());
			Assert::IsTrue(o["b"].isNumber());
			Assert::IsTrue(o["b2"].isPrimitive());
			Assert::IsTrue(o["b"].getBool());
			Assert::IsFalse(o["b2"].getBool());
		}

		TEST_METHOD(JObjectAddShort)
		{
			UObject o;
			o["s"] = (short)10;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["s"].getType() == E_UType::Short);
			Assert::IsTrue(o["s"].isNumber());
			Assert::IsTrue(o["s"].isPrimitive());
			Assert::AreEqual((short)10, o["s"].getShort());
		}

		TEST_METHOD(JObjectAddInt)
		{
			UObject o;
			o["i"] = 33000;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["i"].getType() == E_UType::Int);
			Assert::IsTrue(o["i"].isNumber());
			Assert::IsTrue(o["i"].isPrimitive());
			Assert::AreEqual(33000, o["i"].getInt());
		}

		TEST_METHOD(JObjectAddFloat)
		{
			UObject o;
			o["f"] = (float)3.14;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["f"].getType() == E_UType::Float);
			Assert::IsTrue(o["f"].isNumber());
			Assert::IsTrue(o["f"].isPrimitive());
			Assert::AreEqual((float)3.14, o["f"].getFloat());
		}

		TEST_METHOD(JObjectAddDouble)
		{
			UObject o;
			o["d"] = 3.141592653;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["d"].getType() == E_UType::Double);
			Assert::IsTrue(o["d"].isNumber());
			Assert::IsTrue(o["d"].isPrimitive());
			Assert::AreEqual(3.141592653, o["d"].getDouble());
		}

		TEST_METHOD(JObjectAddString)
		{
			UObject o;
			o["st"] = "Test";
			o["st2"] = "3.14";
			Assert::AreEqual(2, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["st"].getType() == E_UType::String);
			Assert::IsTrue(o["st2"].isNumber());
			Assert::IsFalse(o["st"].isNumber());
			Assert::IsTrue(o["st"].isString());
			Assert::IsTrue(o["st2"].isString());
			Assert::IsTrue(o["st"].isPrimitive());
			Assert::AreEqual(std::string{"Test"}, o["st"].getString());
		}

		TEST_METHOD(JObjectAddObject)
		{
			UObject o;
			UObject o2;
			o2["s"] = "Test";
			o["o"] = o2;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["o"].getType() == E_UType::Object);
			Assert::IsTrue(o["o"].isObject());
			Assert::IsFalse(o.isPrimitive());
			Assert::AreEqual(std::string{ "Test" }, o["o"]["s"].getString());
		}

		TEST_METHOD(JObjectAddArray)
		{
			UObject o;
			UArray a;
			for (int i = 1; i <= 3; i++)
			{
				a.push_back(i);
			}
			o["a"] = a;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["a"].getType() == E_UType::Array);
			Assert::AreEqual(3, o["a"].getInt());
			Assert::IsTrue(o["a"].isArray());
			Assert::IsFalse(a.isPrimitive());
			for (int i = 1; i <= 3; i++)
			{
				Assert::AreEqual(i, o["a"][i - 1].getInt());
			}
		}

		TEST_METHOD(JObjectAddNull)
		{
			UObject o;
			o["n"] = nullptr;
			Assert::AreEqual(1, o.getInt());
			Assert::IsFalse(o.isEmpty());
			Assert::IsTrue(o["n"].getType() == E_UType::Null);
			Assert::IsTrue(o["n"].isNull());
			Assert::IsTrue(o["n"].isPrimitive());
			Assert::IsFalse(o["n"].isNumber());
			Assert::AreEqual(std::string{ "null" }, o["n"].getString());
		}

		TEST_METHOD(JObjectReadString)
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

		TEST_METHOD(JObjectEquality)
		{
			UValue v = UObject{ { "id", 1 }, { "value", "test" } };
			UValue v2 = UObject{ { "id", 1 }, { "value", "test" } };
			UObject o;
			JsonSerializer::deserialize(R"({"id": 1, "value": "test"})", o);
			Assert::IsTrue(v == o);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(JObjectGetString)
		{
			std::string json{ R"({"a": true, "b": 1, "c": 33000, "d": 3.14, "e": 3.141592653, "f": "Test", "g": {"h": "Test"}, "i": [1, 2, 3], "j": null})" };
			UObject o;
			Assert::IsTrue(JsonSerializer::deserialize(json, o));
			Assert::AreEqual(9, o.getInt());
			Assert::AreEqual(std::string{"Object{a: Bool, b: Short, c: Int, d: Double, e: Double, f: String, g: Object, i: Array, j: Null}"}, o.getString());
		}

		TEST_METHOD(JObjectWrite)
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

		TEST_METHOD(JObjectFind)
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

		TEST_METHOD(JObjectInit)
		{
			UObject o{ {"a",1},{"b", false}, {"c", UArray{1,"2", nullptr}} };
			Assert::AreEqual(3, (int)o.size());
			Assert::AreEqual(1, (int)o["a"]);
			Assert::IsFalse((bool)o["b"]);
			Assert::AreEqual(3, (int)((UArray&)o["c"]).size());
			UObject o2{ o };
			Assert::IsTrue(o == o2);
			UValue v = o;
			Assert::IsTrue(v == o);
		}

		TEST_METHOD(JObjectReadError)
		{
			UObject o;
			
			Assert::IsFalse(JsonSerializer::deserialize("", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"("test":value)", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1")", o)); 
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":value})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({test:"value"})", o)); 
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1 "test2":"deux"})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1, "test2": 2,})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":[1,2,3}})", o));
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":[1,2 3])", o)); 
			Assert::IsFalse(JsonSerializer::deserialize(R"({"test":1,2, 3]})", o));
		}
	};
}
