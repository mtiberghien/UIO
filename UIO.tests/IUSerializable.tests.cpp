#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "UIO_All.h"
#include "JsonSerializer.h"

using namespace uio;

namespace UIOTests
{
	class Item: public UJsonSerializable
	{
	public:
		Item() : m_id{ -1 }, m_name("default") {}
		Item(int id, const std::string& name) : m_id(id), m_name(name) {}
		int getId() const
		{
			return m_id;
		}
		std::string getName() const
		{
			return m_name;
		}

		// Inherited via IJsonSerializable
		void toObject(UObject& object) const override
		{
			object["id"] = m_id;
			object["name"] = m_name;
		}
		void fromObject(const UObject& object) override
		{
			m_id = object["id"].getInt(m_id);
			m_name = object["name"].getString(m_name);
		}
		
	private:
		int m_id;
		std::string m_name;
	};

	class Items : public std::vector<Item>, public UJsonSerializable
	{
		// Inherited via IJsonSerializable
		void toObject(UObject& object) const override
		{
			UArray a;
			for (auto& i : *this)
			{
				UObject o;
				i.toObject(o);
				a << o;
			}
			object["devices"] = a;
		}
		void fromObject(const UObject& object) override
		{
			clear();
			const UArray& a = object["devices"].getArray();
			for (const auto& v : a)
			{
				Item i;
				i.fromObject(v.getObject());
				emplace_back(i);
			}
		}
	};

	struct Point
	{
		int x;
		int y;
	};

	class ItemPackage: public UJsonSerializable
	{
	public:
		ItemPackage() :m_name(""), m_location({ 0, 0 }), m_items({}) {}
		ItemPackage(std::string name, const Point& location, const Items& items) :
			m_name(name),
			m_location(location),
			m_items(items)
		{
		}
		std::string getName() const { return m_name; }
		const Point& getLocation() const { return m_location; }
		const Items& getItems() const { return m_items; }

		// Inherited via IJsonSerializable
		void toObject(UObject& object) const override
		{
			object["location"] = UObject{ {"x", m_location.x}, {"y", m_location.y} };
			object["name"] = m_name;
			UArray a;
			for (auto& i : m_items)
			{
				UObject o;
				i.toObject(o);
				a << o;
			}
			object["items"] = a;
		}
		void fromObject(const UObject& object) override
		{
			m_items.clear();
			m_name = object["name"].getString(m_name);
			m_location.x = object.find("location.x").getInt();
			m_location.y = object.find("location.y").getInt();
			const UArray& items = object["items"].getArray();
			for (const auto& v : items)
			{
				Item i;
				i.fromObject(v.getObject());
				m_items.emplace_back(i);
			}
		}
	private:
		std::string m_name;
		Point m_location;
		Items m_items;
	};

	TEST_CLASS(IUSerializabletests)
	{
		TEST_METHOD(ItemSerialization)
		{
			Item i{ 1, "test_1" };
			Item i2{ 2, "test_2" };
			std::string json = i.serialize(false);
			Assert::AreEqual(std::string{ R"({"id": 1, "name": "test_1"})" }, json);
			json = i2.serialize(false);
			Assert::AreEqual(std::string{ R"({"id": 2, "name": "test_2"})" }, json);
		}

		TEST_METHOD(ItemDeserialization)
		{
			Item i;
			Assert::IsTrue(i.deserialize(std::string{ R"({"id": 3, "name": "test_3"})" }));
			Assert::AreEqual(3, i.getId());
			Assert::AreEqual(std::string{ "test_3" }, i.getName());
			Item i2;
			Assert::IsTrue(i2.deserialize(std::string{ R"({"test":"wrong object"})" }));
			Assert::AreEqual(-1, i2.getId());
			Assert::AreEqual(std::string{ "default" }, i2.getName());
		}

		TEST_METHOD(ItemsSerialization)
		{
			Items items;
			items.push_back({ 1,"item_1" });
			items.push_back({ 2, "item_2" });
			std::string json = items.serialize(false);
			Assert::AreEqual(std::string{ R"({"devices": [{"id": 1, "name": "item_1"}, {"id": 2, "name": "item_2"}]})" }, json);
		}

		TEST_METHOD(ItemsDeserialization)
		{
			Items items;
			Assert::IsTrue(items.deserialize(std::string{ R"({"devices": [{"id":1}, {"name": "item_2"}, {"id": 3, "name": "item_3"}]})" }));
			Assert::AreEqual(3, (int)items.size());
			Assert::AreEqual(1, items[0].getId());
			Assert::AreEqual(std::string{ "default" }, items[0].getName());
			Assert::AreEqual(-1, items[1].getId());
			Assert::AreEqual(std::string{ "item_2" }, items[1].getName());
			Assert::AreEqual(3, items[2].getId());
			Assert::AreEqual(std::string{ "item_3" }, items[2].getName());
		}

		TEST_METHOD(ComplexItemSerialization)
		{
			Items items;
			items.push_back({ 1,"one" });
			items.push_back({ 2, "two" });
			ItemPackage p("test", { 10,15 }, items);
			std::string json = p.serialize();
			std::string expected = R"({"items": [{"id": 1, "name": "one"}, {"id": 2, "name": "two"}], "location": {"x": 10, "y": 15}, "name": "test"})";
			Assert::AreEqual(expected, json);
		}

		TEST_METHOD(ComplexItemDeserialization)
		{
			std::string json = R"({"items": [{"id": 1, "name": "one"}, {"id": 2, "name": "two"}], "location": {"x": 10, "y": 15}, "name": "test"})";
			ItemPackage p;
			Assert::IsTrue(p.deserialize(json));
			Assert::AreEqual(std::string{ "test" }, p.getName());
			Assert::AreEqual(10, p.getLocation().x);
			Assert::AreEqual(15, p.getLocation().y);
			Assert::AreEqual(2, p.getItems().at(1).getId());
			Assert::AreEqual(std::string{ "one" }, p.getItems().at(0).getName());
		}


	};
}