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
	object["name"] = getName();
}

static E_MachineType MachineFromString(const std::string& type)
{
	if (type == "D") return E_MachineType::D;
	return E_MachineType::C;
}

void Vehicule::fromObject(const UObject& object)
{
	setName(object["name"].getString("undefined"));
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
	object["name"] = getName();
}

void Machine::readCommonProperties(const UObject& object)
{
	setName(object["name"].getString("undefined"));
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
		v.toObject(o);
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
		push_back(VehiculefromString(v.getName()));
		back().fromObject(v);
	}
}

STD::STD(const Machine& machine)
{
	switch (machine.getType())
	{
	case E_MachineType::C: m_ptr = std::make_unique<MachineC>(dynamic_cast<const MachineC&>(machine)); break;
	case E_MachineType::D: m_ptr = std::make_unique<MachineD>(dynamic_cast<const MachineD&>(machine)); break;
	default: m_ptr = nullptr;
	}
}
STD::STD(E_MachineType type) : m_ptr(Machine::build(type)) {}

SSP::SSP(const Vehicule& vehicule)
{
	switch (vehicule.getType())
	{
	case E_VehiculeType::A: m_ptr = std::make_unique<VehiculeA>(dynamic_cast<const VehiculeA&>(vehicule)); break;
	case E_VehiculeType::B: m_ptr = std::make_unique<VehiculeB>(dynamic_cast<const VehiculeB&>(vehicule)); break;
	default: m_ptr = nullptr;
	}
}

SSP::SSP(E_VehiculeType type) : m_ptr(Vehicule::build(type)) {}

void SSP::toObject(UObject& object) const
{
	m_ptr->toObject(object);
	UArray a;
	for (const auto& m : *this)
	{
		UObject o;
		m.toObject(o);
		a << o;
	}
	object["machines"] = a;
}

void SSP::fromObject(const UObject& object)
{
	m_ptr->fromObject(object);
	clear();
	for (const UObject& m : object["machines"].getArray())
	{
		push_back(MachineFromString(m.getName()));
		back().fromObject(m);
	}
}
