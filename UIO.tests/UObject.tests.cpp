#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UObjecttests)
	{
	public:
		
		TEST_METHOD(UObjectIsEmpty)
		{
			UObject o;
			Assert::IsTrue(o.isEmpty(), L"o doit etre vide");
		}

		TEST_METHOD(UObjectAddBool)
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

		TEST_METHOD(UObjectAddShort)
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

		TEST_METHOD(UObjectAddInt)
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

		TEST_METHOD(UObjectAddFloat)
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

		TEST_METHOD(UObjectAddDouble)
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

		TEST_METHOD(UObjectAddString)
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

		TEST_METHOD(UObjectAddObject)
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

		TEST_METHOD(UObjectAddArray)
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

		TEST_METHOD(UObjectAddNull)
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

		TEST_METHOD(UObjectInit)
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

		TEST_METHOD(UObjectErase)
		{
			UObject o{ {"a", 1},{"b",2},{"c",3} };
			Assert::IsTrue(o.erase("b"));
			Assert::AreEqual(2, (int)o.size());
			Assert::IsFalse(o.erase("b"));
			Assert::IsTrue(o.eraseAt(0));
			Assert::AreEqual(1, (int)o.size());
			Assert::IsFalse(o.eraseAt(2));
			Assert::AreEqual(3, o[0].getInt());
		}

		TEST_METHOD(AffectFromInnerElement)
		{
			UObject o{ {"o", UObject{{"a",1},{"b",2}}} };
			UObject& oRef = o;
			oRef = oRef["o"];
			Assert::AreEqual(2, (int)oRef.size());
			Assert::AreEqual(1, oRef["a"].getInt());
			Assert::AreEqual(2, oRef["b"].getInt());
		}

		TEST_METHOD(GetString)
		{
			UObject o{ {"test", "value"},{"test2",3} };
			Assert::AreEqual(std::string{ "Object{test: String, test2: Int}" }, o.getString());
			o["test2"] = UObject{ {"a", 1} };
			Assert::AreEqual(std::string{ "Object{test: String, test2: Object}" }, o.getString());
			o.setName("test");
			Assert::AreEqual(std::string{ "Object:test{test: String, test2: Object}" }, o.getString());

		}

		TEST_METHOD(Clear)
		{
			UObject o{ {"a", 1},{"b", "test"}, {"c", nullptr} };
			Assert::IsFalse(o.isEmpty());
			Assert::AreEqual(3, o.getInt());
			o.clear();
			Assert::IsTrue(o.isEmpty());
			Assert::AreEqual(0, o.getInt());
		}
	};
}
