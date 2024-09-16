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


	};
}