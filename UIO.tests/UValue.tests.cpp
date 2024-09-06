#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(UValuetests)
	{
		TEST_METHOD(UndefinedValue)
		{
			UValue v;
			Assert::IsTrue(v.getType() == E_UType::Undefined);
			Assert::IsTrue(v.isUndefined());
			UObject o;
			UValue& vRef = o["test"];
			Assert::IsTrue(vRef.isUndefined());

			UArray a;
			a << UValue{};
			Assert::IsTrue(a[0].isUndefined());
		}
		TEST_METHOD(TypeInitialization)
		{
			UValue v;
			E_UType types[] = { E_UType::Array, E_UType::Bool, E_UType::Short, E_UType::Int, E_UType::Float, E_UType::Double, E_UType::String, E_UType::Null, E_UType::Object, E_UType::Undefined};
			for (E_UType t : types)
			{
				v = UValue{ t };
				Assert::AreEqual(toString(t), toString(v.getType()));
			}
		}

		TEST_METHOD(TestIntEquality)
		{
			UValue v = 1;
			UValue v2 = 1;
			Assert::AreEqual(1, (int)v);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestBoolEquality)
		{
			UValue v = true;
			UValue v2 = true;
			Assert::AreEqual(true, (bool)v);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestShortEquality)
		{
			unsigned short s = 1;
			UValue v = (short)1;
			UValue v2 = s;
			Assert::AreEqual((short)1, (short)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestFloatEquality)
		{
			float f = 1;
			UValue v = (float)1;
			UValue v2 = f;
			Assert::AreEqual((float)1, (float)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestDoubleEquality)
		{
			double d = 1;
			UValue v = (double)1;
			UValue v2 = d;
			Assert::AreEqual((double)1, (double)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestStringEquality)
		{
			std::string s{ "Test" };
			UValue v = "Test";
			UValue v2 = s;
			Assert::AreEqual(std::string{"Test"}, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestNullEquality)
		{
			UValue v = nullptr;
			UValue v2 = nullptr;
			Assert::AreEqual(UValue{nullptr}.getString(), v2.getString());
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(ErrorHandling)
		{
			UValue v = 1;
			UObject o;
			UArray a;
			UValue& vError = v[1];
			UObject& vOError = v.getObject();
			UArray& vAError = v.getArray();

			Assert::IsTrue(vError.getType() == E_UType::Error, L"vError doit etre en Error");
			Assert::IsTrue(vOError.getType() == E_UType::Error, L"vOError doit etre en Error");
			Assert::IsTrue(vAError.getType() == E_UType::Error, L"vAError doit etre en Error");

			vError = 1;
			vOError = UObject{ {"id", 1} };
			vAError = UArray{ 1,2,3 };

			Assert::AreEqual(-1, vError.getInt(-1));
			Assert::IsTrue(vError.getType() == E_UType::Error, L"vError doit etre en Error");
			Assert::IsTrue(vOError.isEmpty());
			Assert::IsFalse(vOError == UObject{ {"id", 1 } });
			Assert::IsTrue(vOError.getType() == E_UType::Error, L"vOError doit etre en Error");
			Assert::IsTrue(vAError.isEmpty());
			Assert::IsFalse(vAError == UArray{ 1,2,3 });
			Assert::IsTrue(vAError.getType() == E_UType::Error, L"vAError doit etre en Error");

			UValue vCopyError = vError;
			UObject vOCopyError = vOError;
			UArray vACopyError = vAError;
			Assert::IsTrue(vCopyError.getType() == E_UType::Undefined, L"vCopyError doit etre Undefined");
			Assert::IsTrue(vOCopyError.getType() == E_UType::Object, L"vOCopyError doit etre Object");
			Assert::IsTrue(vACopyError.getType() == E_UType::Array, L"vACopyError doit etre Array");
			o["v"] = vError;
			o["vO"] = vOError;
			o["vA"] = vAError;
			Assert::AreEqual((std::size_t)3, o.size());
			Assert::IsTrue(o["v"].getType() == E_UType::Undefined, L"vCopyError doit etre Undefined");
			Assert::IsTrue(o["vO"].getType() == E_UType::Object, L"vOCopyError doit etre Object");
			Assert::IsTrue(o["vA"].getType() == E_UType::Array, L"vACopyError doit etre Array");
			o["v"] = 1;
			o["vO"]["test"] = 1;
			o["vA"].getArray() << 1;
			Assert::AreEqual(1, o["v"].getInt());
			Assert::AreEqual(1, o["vO"]["test"].getInt());
			Assert::AreEqual(1, o["vA"][0].getInt());
		}

		TEST_METHOD(CopyHandling)
		{
			UValue v = 1;
			UValue v2 = v;
			Assert::IsTrue(v == v2);
			Assert::AreEqual(1, v2.getInt());
			v = UObject{ {"id", 1} };
			v2 = v;
			Assert::IsTrue(v == v2);
			Assert::AreEqual(UObject{ {"id", 1} }.getString(), v2.getString());
			UValue& v2Ref = v2;
			v2["test"] = "test";
			Assert::AreEqual(2, v2.getInt());
			Assert::AreEqual(1, v.getInt());
			Assert::AreEqual(2, v2.getInt());

			v = UArray{ 1,2,3 };
			v2 = v;

			Assert::AreEqual(UArray{ 1,2,3 }.getString(), v.getArray().getString());
			v2.getArray() << 4;
			Assert::AreEqual(4, v2.getInt());
			Assert::AreEqual(3, v.getInt());
			Assert::AreEqual(4, v2.getInt());

			UObject o;
			o["a"] = v2;
			UObject o2;
			o2["a"] = v2Ref;
			v2.getArray() << 5;
			Assert::AreEqual(4, o["a"].getInt());
			Assert::AreEqual(4, o2["a"].getInt());
			Assert::AreEqual(5, v2Ref.getInt());
		}

		TEST_METHOD(DeepCopyHandling)
		{
			UObject o{ {"id", 1}, {"o2", UObject{{"id",2}}} };
			UValue v1 = o;
			UValue v2 = o;
			Assert::AreEqual(1, v1["id"].getInt());
			Assert::AreEqual(1, v2["id"].getInt());
			v1["id"] = 2;
			v2["id"] = 3;
			Assert::AreEqual(1, o["id"].getInt());
			Assert::AreEqual(2, v1["id"].getInt());
			Assert::AreEqual(3, v2["id"].getInt());
			o.find("o2.id") = 4;
			v2["o2"]["id"] = 3;
			Assert::AreEqual(4, o["o2"]["id"].getInt());
			Assert::AreEqual(2, v1.getObject().find("o2.id").getInt());
			Assert::AreEqual(3, v2["o2"]["id"].getInt());

			UArray a{ o, v1, v2 };

		}

		TEST_METHOD(findValue)
		{
			UValue v = UObject{ { "array", UArray{ 1, UObject{ { "id", 2 } } } } };
			Assert::AreEqual(2, v.find("array[1].id").getInt());
			v = UArray{ { UArray{ 1, { UObject{ { "id", 2 } } } } } };
			Assert::AreEqual(2, v.find("[0][1].id").getInt());
		}
	};
}