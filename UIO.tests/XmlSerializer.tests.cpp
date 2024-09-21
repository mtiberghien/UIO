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
			Assert::AreEqual(std::string{ R"(<Object xmlns:uio="urn:uio:schema" a="test" b="2"><C uio:key="c" uio:type="Array"><Int>1</Int><String>deux</String><Item uio:class="item" value="3"/><Null/></C></Object>)" }, xml);
		}

		TEST_METHOD(SerializeArray)
		{
			UArray a;
			std::string xml = XmlSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ R"(<Array xmlns:uio="urn:uio:schema" uio:type="Array"/>)" }, xml);
			a << 2.5 << (float)2.3 << (short)3 << true;
			xml = XmlSerializer::serialize(a, false);
			Assert::AreEqual(std::string{ R"(<Array xmlns:uio="urn:uio:schema" uio:type="Array"><Double>2.5</Double><Float>2.3</Float><Short>3</Short><Bool>true</Bool></Array>)" }, xml);
		}

		TEST_METHOD(DeserializeObject)
		{
			UObject o;
			std::string xml = R"(<Object xmlns:uio="urn:uio:schema" a="test" b="2"><C uio:key="c" uio:type="Array"><Int>1</Int><String>deux</String><Item uio:class="item" value="3"/><Null/></C></Object>)";
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

		TEST_METHOD(DeserializeWebExample)
		{
			std::string xml = R"(<note>
<to>Tove</to>
<from>Jani</from>
<heading>Reminder</heading>
<body>Don't forget me this weekend!</body>
</note>)";
			UObject o;
			Assert::IsTrue(XmlSerializer::deserialize(xml, o));
			Assert::AreEqual(4, o.getInt());
			Assert::AreEqual(std::string{ "Tove" }, o["to"].getString());
			Assert::AreEqual(std::string{ "Jani" }, o["from"].getString());
			Assert::AreEqual(std::string{ "Reminder" }, o["heading"].getString());
			Assert::AreEqual(std::string{ "Don't forget me this weekend!" }, o["body"].getString());
		}

		TEST_METHOD(DeserializeWebExample2)
		{
			std::string xml = R"(<CATALOG>
<PLANT>
<COMMON>Bloodroot</COMMON>
<BOTANICAL>Sanguinaria canadensis</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$2.44</PRICE>
<AVAILABILITY>031599</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Columbine</COMMON>
<BOTANICAL>Aquilegia canadensis</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$9.37</PRICE>
<AVAILABILITY>030699</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Marsh Marigold</COMMON>
<BOTANICAL>Caltha palustris</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Sunny</LIGHT>
<PRICE>$6.81</PRICE>
<AVAILABILITY>051799</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Cowslip</COMMON>
<BOTANICAL>Caltha palustris</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$9.90</PRICE>
<AVAILABILITY>030699</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Dutchman's-Breeches</COMMON>
<BOTANICAL>Dicentra cucullaria</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$6.44</PRICE>
<AVAILABILITY>012099</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Ginger, Wild</COMMON>
<BOTANICAL>Asarum canadense</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$9.03</PRICE>
<AVAILABILITY>041899</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Hepatica</COMMON>
<BOTANICAL>Hepatica americana</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$4.45</PRICE>
<AVAILABILITY>012699</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Liverleaf</COMMON>
<BOTANICAL>Hepatica americana</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$3.99</PRICE>
<AVAILABILITY>010299</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Jack-In-The-Pulpit</COMMON>
<BOTANICAL>Arisaema triphyllum</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$3.23</PRICE>
<AVAILABILITY>020199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Mayapple</COMMON>
<BOTANICAL>Podophyllum peltatum</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$2.98</PRICE>
<AVAILABILITY>060599</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Phlox, Woodland</COMMON>
<BOTANICAL>Phlox divaricata</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$2.80</PRICE>
<AVAILABILITY>012299</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Phlox, Blue</COMMON>
<BOTANICAL>Phlox divaricata</BOTANICAL>
<ZONE>3</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$5.59</PRICE>
<AVAILABILITY>021699</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Spring-Beauty</COMMON>
<BOTANICAL>Claytonia Virginica</BOTANICAL>
<ZONE>7</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$6.59</PRICE>
<AVAILABILITY>020199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Trillium</COMMON>
<BOTANICAL>Trillium grandiflorum</BOTANICAL>
<ZONE>5</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$3.90</PRICE>
<AVAILABILITY>042999</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Wake Robin</COMMON>
<BOTANICAL>Trillium grandiflorum</BOTANICAL>
<ZONE>5</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$3.20</PRICE>
<AVAILABILITY>022199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Violet, Dog-Tooth</COMMON>
<BOTANICAL>Erythronium americanum</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$9.04</PRICE>
<AVAILABILITY>020199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Trout Lily</COMMON>
<BOTANICAL>Erythronium americanum</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$6.94</PRICE>
<AVAILABILITY>032499</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Adder's-Tongue</COMMON>
<BOTANICAL>Erythronium americanum</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$9.58</PRICE>
<AVAILABILITY>041399</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Anemone</COMMON>
<BOTANICAL>Anemone blanda</BOTANICAL>
<ZONE>6</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$8.86</PRICE>
<AVAILABILITY>122698</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Grecian Windflower</COMMON>
<BOTANICAL>Anemone blanda</BOTANICAL>
<ZONE>6</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$9.16</PRICE>
<AVAILABILITY>071099</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Bee Balm</COMMON>
<BOTANICAL>Monarda didyma</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$4.59</PRICE>
<AVAILABILITY>050399</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Bergamot</COMMON>
<BOTANICAL>Monarda didyma</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$7.16</PRICE>
<AVAILABILITY>042799</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Black-Eyed Susan</COMMON>
<BOTANICAL>Rudbeckia hirta</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Sunny</LIGHT>
<PRICE>$9.80</PRICE>
<AVAILABILITY>061899</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Buttercup</COMMON>
<BOTANICAL>Ranunculus</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$2.57</PRICE>
<AVAILABILITY>061099</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Crowfoot</COMMON>
<BOTANICAL>Ranunculus</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$9.34</PRICE>
<AVAILABILITY>040399</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Butterfly Weed</COMMON>
<BOTANICAL>Asclepias tuberosa</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Sunny</LIGHT>
<PRICE>$2.78</PRICE>
<AVAILABILITY>063099</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Cinquefoil</COMMON>
<BOTANICAL>Potentilla</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$7.06</PRICE>
<AVAILABILITY>052599</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Primrose</COMMON>
<BOTANICAL>Oenothera</BOTANICAL>
<ZONE>3 - 5</ZONE>
<LIGHT>Sunny</LIGHT>
<PRICE>$6.56</PRICE>
<AVAILABILITY>013099</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Gentian</COMMON>
<BOTANICAL>Gentiana</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$7.81</PRICE>
<AVAILABILITY>051899</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Blue Gentian</COMMON>
<BOTANICAL>Gentiana</BOTANICAL>
<ZONE>4</ZONE>
<LIGHT>Sun or Shade</LIGHT>
<PRICE>$8.56</PRICE>
<AVAILABILITY>050299</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Jacob's Ladder</COMMON>
<BOTANICAL>Polemonium caeruleum</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$9.26</PRICE>
<AVAILABILITY>022199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Greek Valerian</COMMON>
<BOTANICAL>Polemonium caeruleum</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$4.36</PRICE>
<AVAILABILITY>071499</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>California Poppy</COMMON>
<BOTANICAL>Eschscholzia californica</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Sun</LIGHT>
<PRICE>$7.89</PRICE>
<AVAILABILITY>032799</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Shooting Star</COMMON>
<BOTANICAL>Dodecatheon</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Mostly Shady</LIGHT>
<PRICE>$8.60</PRICE>
<AVAILABILITY>051399</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Snakeroot</COMMON>
<BOTANICAL>Cimicifuga</BOTANICAL>
<ZONE>Annual</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$5.63</PRICE>
<AVAILABILITY>071199</AVAILABILITY>
</PLANT>
<PLANT>
<COMMON>Cardinal Flower</COMMON>
<BOTANICAL>Lobelia cardinalis</BOTANICAL>
<ZONE>2</ZONE>
<LIGHT>Shade</LIGHT>
<PRICE>$3.02</PRICE>
<AVAILABILITY>022299</AVAILABILITY>
</PLANT>
</CATALOG>)";

		UValue v;
		Assert::IsTrue(XmlSerializer::deserialize(xml, v));
		Assert::AreEqual(toString(E_UType::Array), toString(v.getType()));
		Assert::AreEqual(36, v.getInt());
		Assert::AreEqual(toString(E_UType::Object), toString(v[35].getType()));
		UObject& o = v[35].getObject();
		Assert::AreEqual(6, o.getInt());
		Assert::AreEqual(std::string{ "Cardinal Flower" }, o["COMMON"].getString());
		Assert::AreEqual(std::string{ "Lobelia cardinalis" }, o["BOTANICAL"].getString());
		Assert::AreEqual(2, o["ZONE"].getInt());
		Assert::AreEqual(toString(E_UType::Short), toString(o["ZONE"].getType()));
		}

        TEST_METHOD(DeserializeWebExample3)
        {
            std::string xml = R"(<?xml version="1.0"?>
<catalog>
   <book id="bk101">
      <author>Gambardella, Matthew</author>
      <title>XML Developer's Guide</title>
      <genre>Computer</genre>
      <price>44.95</price>
      <publish_date>2000-10-01</publish_date>
      <description>An in-depth look at creating applications 
      with XML.</description>
   </book>
   <book id="bk102">
      <author>Ralls, Kim</author>
      <title>Midnight Rain</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2000-12-16</publish_date>
      <description>A former architect battles corporate zombies, 
      an evil sorceress, and her own childhood to become queen 
      of the world.</description>
   </book>
   <book id="bk103">
      <author>Corets, Eva</author>
      <title>Maeve Ascendant</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2000-11-17</publish_date>
      <description>After the collapse of a nanotechnology 
      society in England, the young survivors lay the 
      foundation for a new society.</description>
   </book>
   <book id="bk104">
      <author>Corets, Eva</author>
      <title>Oberon's Legacy</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2001-03-10</publish_date>
      <description>In post-apocalypse England, the mysterious 
      agent known only as Oberon helps to create a new life 
      for the inhabitants of London. Sequel to Maeve 
      Ascendant.</description>
   </book>
   <book id="bk105">
      <author>Corets, Eva</author>
      <title>The Sundered Grail</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2001-09-10</publish_date>
      <description>The two daughters of Maeve, half-sisters, 
      battle one another for control of England. Sequel to 
      Oberon's Legacy.</description>
   </book>
   <book id="bk106">
      <author>Randall, Cynthia</author>
      <title>Lover Birds</title>
      <genre>Romance</genre>
      <price>4.95</price>
      <publish_date>2000-09-02</publish_date>
      <description>When Carla meets Paul at an ornithology 
      conference, tempers fly as feathers get ruffled.</description>
   </book>
   <book id="bk107">
      <author>Thurman, Paula</author>
      <title>Splish Splash</title>
      <genre>Romance</genre>
      <price>4.95</price>
      <publish_date>2000-11-02</publish_date>
      <description>A deep sea diver finds true love twenty 
      thousand leagues beneath the sea.</description>
   </book>
   <book id="bk108">
      <author>Knorr, Stefan</author>
      <title>Creepy Crawlies</title>
      <genre>Horror</genre>
      <price>4.95</price>
      <publish_date>2000-12-06</publish_date>
      <description>An anthology of horror stories about roaches,
      centipedes, scorpions  and other insects.</description>
   </book>
   <book id="bk109">
      <author>Kress, Peter</author>
      <title>Paradox Lost</title>
      <genre>Science Fiction</genre>
      <price>6.95</price>
      <publish_date>2000-11-02</publish_date>
      <description>After an inadvertant trip through a Heisenberg
      Uncertainty Device, James Salway discovers the problems 
      of being quantum.</description>
   </book>
   <book id="bk110">
      <author>O'Brien, Tim</author>
      <title>Microsoft .NET: The Programming Bible</title>
      <genre>Computer</genre>
      <price>36.95</price>
      <publish_date>2000-12-09</publish_date>
      <description>Microsoft's .NET initiative is explored in 
      detail in this deep programmer's reference.</description>
   </book>
   <book id="bk111">
      <author>O'Brien, Tim</author>
      <title>MSXML3: A Comprehensive Guide</title>
      <genre>Computer</genre>
      <price>36.95</price>
      <publish_date>2000-12-01</publish_date>
      <description>The Microsoft MSXML3 parser is covered in 
      detail, with attention to XML DOM interfaces, XSLT processing, 
      SAX and more.</description>
   </book>
   <book id="bk112">
      <author>Galos, Mike</author>
      <title>Visual Studio 7: A Comprehensive Guide</title>
      <genre>Computer</genre>
      <price>49.95</price>
      <publish_date>2001-04-16</publish_date>
      <description>Microsoft Visual Studio 7 is explored in depth,
      looking at how Visual Basic, Visual C++, C#, and ASP+ are 
      integrated into a comprehensive development 
      environment.</description>
   </book>
</catalog>)";

            UValue v;
            Assert::IsTrue(XmlSerializer::deserialize(xml, v));
            Assert::IsTrue(v.isArray(), L"v doit être un Array");
            Assert::AreEqual(12, v.getInt());
            UObject& o = v[10];
            Assert::AreEqual(7, o.getInt());
            Assert::AreEqual(std::string{ "book" }, o.getClass());
            Assert::AreEqual(std::string{ "2000-12-01" }, o["publish_date"].getString());
        }
	};
}