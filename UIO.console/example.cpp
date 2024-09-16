#include "example.h"


std::unique_ptr<Vehicule> Vehicule::build(E_VehiculeType type)
{
	switch (type)
	{
	case E_VehiculeType::A: return std::make_unique<VehiculeA>();
	case E_VehiculeType::B: return std::make_unique<VehiculeB>();
	}
	return nullptr;
}

void Vehicule::toObject(UObject& object) const
{
	UArray a;
	for (const auto& m : *this)
	{
		UObject o;
		m->toObject(o);
		a << o;
	}
	object["machines"] = a;
	object["name"] = m_name;
}

static E_MachineType MachineFromString(const std::string& type)
{
	if (type == "D") return E_MachineType::D;
	return E_MachineType::C;
}

void Vehicule::fromObject(const UObject& object)
{
	clear();
	m_name = object["name"].getString("undefined");
	for (const UObject& m : object["machines"].getArray())
	{
		push_back(Machine::build(MachineFromString(m.getName())));
		back()->fromObject(m);
	}
}

std::unique_ptr<Machine> Machine::build(E_MachineType type)
{
	switch (type)
	{
	case E_MachineType::C: return std::make_unique<MachineC>();
	case E_MachineType::D: return std::make_unique<MachineD>();
	}
	return nullptr;
}

void Machine::toObject(UObject& object) const
{
	writeCommonProperties(object);
}

void Machine::fromObject(const UObject& object)
{
	readCommonProperties(object);
}

void Machine::writeCommonProperties(UObject& object) const
{
	object["name"] = m_name;
}

void Machine::readCommonProperties(const UObject& object)
{
	m_name = object["name"].getString("undefined");
}

void MachineC::fromObject(const UObject& object)
{
	readCommonProperties(object);
	m_value = object["value"].getInt(-1);
}

void MachineC::toObject(UObject& object) const
{
	object.setName("C");
	writeCommonProperties(object);
	object["value"] = m_value;
}

void MachineD::fromObject(const UObject& object)
{
	readCommonProperties(object);
	m_isWorking = object["isWorking"].getInt(-1);
}

void MachineD::toObject(UObject& object) const
{
	object.setName("D");
	writeCommonProperties(object);
	object["isWorking"] = m_isWorking;
}

void Flotte::toObject(UObject& object) const
{
	object.setName("flotte");
	UArray a;
	for (const auto& v : *this)
	{
		UObject o;
		v->toObject(o);
		a << o;
	}
	object["vehicules"] = a;
}

static E_VehiculeType VehiculefromString(const std::string& type)
{
	if (type == "B") return E_VehiculeType::B;
	return E_VehiculeType::A;
}

void Flotte::fromObject(const UObject& object)
{
	clear();
	for (const UObject& v : object["vehicules"].getArray())
	{
		push_back(Vehicule::build(VehiculefromString(v.getName())));
		back()->fromObject(v);
	}
}
