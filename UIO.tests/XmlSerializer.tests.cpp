#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"
#include "XmlSerializer.h"

using namespace uio;

namespace UIOTests
{
	TEST_CLASS(XmlSerializerTests)
	{
		TEST_METHOD(SerializeObject)
		{
			UObject o;
			std::string xml = XmlSerializer::serialize(o, false);
			Assert::AreEqual(std::string{ R"(<Object xmlns:uio="urn:uio:schema"/>)" }, xml);
			o["a"] = "test";
			o["b"] = 2;
			o["c"] = UArray{ 1, "deux", UObject("item",{{"value", 3}}), nullptr};
			xml = XmlSerializer::serialize(o, false);
			Assert::AreEqual(std::string{ R"(<Object xmlns:uio="urn:uio:schema" a="test" b="2"><C uio:key="c"><Int>1</Int><String>deux</String><Item uio:class="item" value="3"/><Null/></C></Object>)" }, xml);
		}

		TEST_METHOD(SerializeArray)
		{
			UArray a;
			std::string xml = XmlSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ R"(<Array xmlns:uio="urn:uio:schema"/>)" }, xml);
			a << 2.5 << (float)2.3 << (short)3 << true;
			xml = XmlSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ R"(<Array xmlns:uio="urn:uio:schema"><Double>2.5</Double><Float>2.3</Float><Short>3</Short><Bool>true</Bool></Array>)" }, xml);
		}

		TEST_METHOD(DeserializeObject)
		{
			UObject o;
			std::string xml = R"(<Object xmlns:uio="urn:uio:schema" a="test" b="2"><C uio:key="c"><Int>1</Int><String>deux</String><Item uio:class="item" value="3"/><Null/></C></Object>)";
			Assert::IsTrue(XmlSerializer::deserialize(xml, o));
			Assert::AreEqual(3, o.getInt());
			Assert::AreEqual(std::string{ "test" }, (std::string)o["a"]);
			Assert::AreEqual(2, o["b"].getInt());
			Assert::AreEqual(toString(E_UType::Array), toString(o["c"].getType()));
			Assert::AreEqual(1, o.find("c[0]").getInt());
			Assert::AreEqual(std::string{ "deux" }, o["c"][1].getString());
			Assert::AreEqual(toString(E_UType::Object), toString(o["c"][2].getType()));
			Assert::AreEqual(3, o.find("c[2].value").getInt());
			Assert::AreEqual(std::string{ "item" }, o["c"][2].getObject().getClass());
			Assert::IsTrue(o["c"][3].isNull());
		}

		TEST_METHOD(DeserializeArray)
		{
			UArray a;
			std::string xml = R"(<Array xmlns:uio="urn:uio:schema"><Double>2.5</Double><Float>2.3</Float><Short>3</Short><Bool>true</Bool></Array>)";
			Assert::IsTrue(XmlSerializer::deserialize(xml, a));
			Assert::AreEqual(4, a.getInt());
			Assert::AreEqual(toString(E_UType::Double), toString(a[0].getType()));
			Assert::AreEqual(2.5, a[0].getDouble());
			Assert::AreEqual(toString(E_UType::Float), toString(a[1].getType()));
			Assert::AreEqual((float)2.3, a[1].getFloat());
			Assert::AreEqual(toString(E_UType::Short), toString(a[2].getType()));
			Assert::AreEqual((short)3, a[2].getShort());
			Assert::AreEqual(toString(E_UType::Bool), toString(a[3].getType()));
			Assert::IsTrue(a[3].getBool());
		}
	};
}