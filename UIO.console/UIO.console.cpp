// JsonModele.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "UIO_All.h"
#include "JsonSerializer.h"
#include "XmlSerializer.h"
#include "IniSerializer.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include "example.h"
#include "Windows.h"

using namespace uio;


class Item : public IUSerializable
{
public:
    Item() {}
    Item(const std::string& name, int id) : m_name(name), m_id(id) {}
    void toObject(UObject& object) const override
    {
        object.setName("item");
        object["name"] = m_name;
        object["id"] = m_id;
    }
    void fromObject(const UObject& object) override
    {
        m_name = object["name"].getString("default");
        m_id = object["id"].getInt(-1);
    }
    std::string getName() const { return m_name; }
    int getId() const { return m_id; }
private:
    std::string m_name;
    int m_id{ 0 };
};

class Items : public UJsonSerializable
{
public:
    Items()
    {
    }
    void toObject(UObject& object) const override
    {
        object.setName("items");
        UArray l_devices;
        for (auto& d : m_items)
        {
            UObject o;
            d.toObject(o);
            l_devices.push_back(o);
        }
        object["devices"] = l_devices;
    }

    void fromObject(const UObject& object) override
    {
        m_items.clear();
        const UArray& items = object["devices"].getArray();
        for (const auto& v : items)
        {
            const UObject& o = v.getObject();
            Item it;
            it.fromObject(o);
            add(it);
        }
    }

    void add(const Item& d)
    {
        m_items.push_back(d);
    }
    void add(Item&& d)
    {
        m_items.push_back(d);
    }
    Item& operator[](int index)
    {
        return m_items[index];
    }
    std::string to_string()
    {
		return JsonSerializer::serialize(*this, false);
    }
private:
    std::vector<Item> m_items;
};

std::ostream& operator<<(std::ostream& stream, E_UType type)
{
    stream << toString(type);
    return stream;
}



void TestValue(const UValue& v, E_UType type=E_UType::String)
{
    std::cout << v.getType() << ": ";
    switch (type)
    {
    case E_UType::Bool: std::cout << v.getBool(); break;
    case E_UType::Short: std::cout << v.getShort(); break;
    case E_UType::Int: std::cout << v.getInt(); break;
    case E_UType::Float: std::cout << v.getFloat(); break;
    case E_UType::Double: std::cout << std::setprecision(12) << v.getDouble(); break;
    case E_UType::String: std::cout << v.getString(); break;
    }
    std::cout << std::endl;
}

void ChangeValue(UValue& v)
{
    v = "test";
}

#include <fstream>

void TestJObject()
{
    UObject o;
    o["isOk"] = true;
    o["value"] = 3500;
	o["item"] = E_UType::Object;
	JsonSerializer::deserialize(R"({"id": 1, "name": "item"})", o["item"].getArray());
    o["price"] = 35.78;
    UArray a;
    UArray a2{ 10, "20", o["item"], nullptr };
    for (int i = 1; i < 4; i++)
    {
        a << UObject{ {"id", i} };
    }
    o["ids"] = a;
    o["values"] = a2;
    std::cout << o.getString() << std::endl;
    JsonSerializer::serialize(std::cout, o, true);
    std::cout << "ids[0]: " << o["ids"][0].getString() << std::endl;
	std::cout << "o[0][\"0\"]: ";
	JsonSerializer::serialize(std::cout, o[0]["0"], false);
	std::cout << std::endl;
    std::cout << "isOk: " << (std::string)o["isOk"] << std::endl;
    std::cout << "values: " << o["values"].getString() << std::endl;
    std::cout << "values[1]: " << (int)o["values"][1] << std::endl;
    std::cout << "ids[2].id: " << o.find("ids[2].id").getInt() << std::endl;
    std::cout << "price: " << o["price"].getDouble() << std::endl;
    std::cout << "test default: " << o.find("path.does.not.exist").getString("not found") << std::endl;

    // Expected results
    /*

		Object{ids: Array, isOk: Bool, item: Object, price: Double, value: Int, values: Array}
		{
			"ids":
			[
				{
				"id": 1
				},
				{
				"id": 2
				},
				{
				"id": 3
				}
			],
			"isOk": true,
			"item": {},
			"price": 35.78,
			"value": 3500,
			"values":
			[
				10,
				"20",
				{},
				null
			]
		}
		ids[0]: Object{id: Int}
		o[0]["0"]: {"id": 1}
		isOk: true
		values: Array[4]
		values[1]: 20
		ids[2].id: 3
		price: 35.78
		test default: not found
    */
}



void TestSerialization()
{
    Items d;
    d.add({ "device_1", 1 });
    d.add({ "device_2" , 2 });
    JsonSerializer::serialize(std::cout, d, true);
    Items d2;
    std::string djson = JsonSerializer::serialize(d, false);
    JsonSerializer::deserialize(djson, d2);
    d2.add({ "device_3", 3 });
    std::cout << d2.to_string() << std::endl;
    std::cout << "items[1]: " << d2[1].getName() << std::endl;

    // Expected results
    /*
    {
        "devices":
        [
            {
               "id": 1,
               "name": "device_1"
            },
            {
                "id": 2,
                "name": "device_2"
            }
        ]
    }
    {"devices": [{"id": 1, "name": "device_1"}, {"id": 2, "name": "device_2"}, {"id": 3, "name": "device_3"}]}
    items[1]: device_2
    */
}

void setValue(UValue& v)
{
    UValue value = 1;
    v = value;
}

static void Example()
{
    Flotte f;
    MachineD d{ "Machine D", true };
    std::string json = R"({"vehicules": [{"name": "Véhicule A", "uio:name" : "A", "machines" :[{"name": "Machine C", "uio:name": "C", "value": 10}}]}]})";
    JsonSerializer::deserialize(json, f);
    f.push_back(VehiculeB("Véhicule B"));
    f[1].push_back(d);
    f[1].push_back(MachineC("MC", 5));
    json = JsonSerializer::serialize(f, true);
    std::cout << json;
    Flotte f2;
    JsonSerializer::deserialize(json, f2);
    for (const auto& v : f2)
    {
        std::cout << "Véhicule: " << v.getName() << ", " << v.getType() << std::endl;
        std::cout << "\tMachines: " << std::endl;
        for (const auto& m : v)
        {
            std::cout << "\t " << m.getName() << ", " << m.getType() << std::endl;
        }
    }
    XmlSerializer::serialize(std::cout, f2);
    IniSerializer::serialize(std::cout, f2);
}

int main()
{
    setlocale(LC_ALL, "en-US");
   E_UType types[] = { E_UType::Bool, E_UType::Short, E_UType::Int, E_UType::Float, E_UType::Double, E_UType::String };
    UObject o;
    UObject so;
    UArray ta;
    ta.push_back(1);
    ta.push_back("test");
    so["hello"] = "hello\nworld";
    o["b"] = true;
    o["s"] = 2;
    o["i"] = 400000;
    o["f"] = 3.14;
    o["d"] = 3.141592653;
    o["st"] = "Hello";
    o["o"] = so;
    o["t"] = ta;
    o["n"] = nullptr;
    for (auto t : types)
    {
        std::cout << "\tGet" << t << std::endl;
        unsigned short s = 7;
        TestValue(true, t);
        TestValue(false, t);
        TestValue(s, t);
        TestValue(2024, t);
        TestValue(-2024, t);
        TestValue((float)3.14, t);
        TestValue(3.141592653, t);
        TestValue("Hello", t);
        TestValue("123.456", t);
        TestValue("", t);
        TestValue(o, t);
        TestValue(ta, t);
        TestValue(nullptr, t);
    }

    UValue v(1);
    TestValue(v);
    ChangeValue(v);
    TestValue(v);

    TestValue(o["o"]["hello"]);
    TestValue(o[1]);
    UObject& oRef = o["o"].getObject();
    UArray& tRef = o["t"].getArray();
    std::string ojson = o.getString();
    UObject o2;
	JsonSerializer::deserialize(ojson, o2);
    TestValue(o2["o"]["hello"]);
    oRef.clear();
    tRef.clear();
    for (int i = 0; i < 3; i++)
    {
        tRef.push_back(i);
    }
    oRef["tab"] = ta;
	oRef["o2"] = E_UType::Object;
	JsonSerializer::deserialize(R"({"a": 1, "b": "Test", "c": [1, "deux", {"d":true}]})", oRef["o2"].getObject());
    o[10] = "test";
    TestValue(o[11]);
    TestValue(ta["1"]);
    TestValue(ta[0]);
    TestValue(ta[3]);
	JsonSerializer::serialize(std::cout, o, true);
    TestValue(o.find("o.o2.c[2].d"));
    std::cout << o["t"].getString() << std::endl;
    std::cout << ta.getString() << std::endl;
    TestValue(o.find("o.tab[1]"));
    o.clear();
	JsonSerializer::serialize(std::cout, o, true);
    
    TestSerialization();
    TestJObject();

    UObject obj;
    UValue& v1 = obj.find("test.a");
    v1 = 3;
    UValue& v2 = obj.find("test.b");
    TestValue(v1);
    TestValue(v2);

    o["n"] = nullptr;

	UArray aTest{ 1, "2", UObject{{ "item", 3 } }};
    aTest << nullptr << UArray{ 1,2,3 };
    TestValue(aTest);
    aTest[3] = 4;
    TestValue(aTest);
    UObject oTest{ {"id", 1}, {"deux" , 2.0}, {"n", nullptr}, {"a", UArray{1,2,3}}, {"d", E_UType::Double} };
    TestValue(oTest);
    std::map<std::string, UValue> list{ {"id", 1}, {"2", "deux"} };
    for (std::pair<const std::string&, const UValue&> p : list)
    {
        std::cout << p.first << ": " << p.second.getString() << std::endl;
    }

    UObject& oError = UObjectProvider::getObjectError();
    TestValue(oError);
    oError = UObject{ {"test", "c'est un test"} };
    TestValue(oError);
    TestValue(UObjectProvider::getObjectError());

    UArray& aError = UArrayProvider::getArrayError();
    TestValue(aError);
    aError = UArray{ 1,2,3 };
    TestValue(aError);
    TestValue(UArrayProvider::getArrayError());

    UValue vFind = oTest.find("id");
    TestValue(vFind);
    vFind = oTest.find("notFound");
    TestValue(vFind);
    TestValue(oTest["id"]);
    UObject school{ {"teachers", E_UType::Array}, {"students", E_UType::Array} };
    school.setName("school");
    school["students"].getArray() << UObject{{"name", "Frédéric Jamard"},{"age", 16}} << UObject{{"name", "John Charte"},{"age", 15}};
    school["teachers"].getArray() << UObject("teacher",{{"name", "Thierry Delcourt"}, {"subject", "Mathematics"}}) << UObject("teacher",{{"name", "Bernard Artman"}, {"subject", "French"}});
    school["notes"] = UArray{ 12,"16", UObject{{"note", 15.4}},20,18 };
    UArray& students = school["students"].getArray();
    for (UObject& o : students)
    {
        o.setName("student");
    }
    std::string xmlString = XmlSerializer::serialize(school);
    UObject xmlObject;
    std::cout << xmlString << std::endl;
    XmlSerializer::deserialize(xmlString, xmlObject);
    XmlSerializer::serialize(std::cout, xmlObject);
    std::string xmlJsonString = JsonSerializer::serialize(xmlObject, true);
    std::cout << xmlJsonString;
    UObject xmlJsonObject;
    JsonSerializer::deserialize(xmlJsonString, xmlJsonObject);
    XmlSerializer::serialize(std::cout, xmlJsonObject);
    std::string iniString = IniSerializer::serialize(xmlObject);

    IniSerializer::serialize(std::cout, xmlObject);
    std::fstream fIni{ "Test.ini" };
    IniSerializer::serialize(fIni, xmlObject);
    UObject iniObject;
    IniSerializer::deserialize(iniString, iniObject, true);
    XmlSerializer::serialize(std::cout, iniObject, true);
    char iniValue[100];
    GetPrivateProfileStringA("school.notes.2", "note", "default", iniValue, 100, R"(C:\Users\Mathias\Documents\dev\cpp\UIO\UIO.console\Test.ini)");
    std::cout << "school.notes.2: " << iniValue << std::endl;
    Example();
    //std::getchar();
}
