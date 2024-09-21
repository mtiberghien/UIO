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
	object.setClass(::toString(getType()));
	Named::toObject(object);
}

static E_MachineType MachineFromString(const std::string& type)
{
	if (type == "D") return E_MachineType::D;
	return E_MachineType::C;
}

void Vehicule::fromObject(const UObject& object)
{
	Named::fromObject(object);
}

std::string Vehicule::toString() const
{
	std::ostringstream s;
	s << Named::toString() << ", type: " << ::toString(getType());
	return s.str();
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
	Named::toObject(object);
	object.setClass(::toString(getType()));
}

std::string Machine::toString() const
{
	std::ostringstream s;
	s << Named::toString() << ", type: " << ::toString(getType());
	return s.str();
}

void Machine::readCommonProperties(const UObject& object)
{
	Named::fromObject(object);
}

void MachineC::fromObject(const UObject& object)
{
	readCommonProperties(object);
	m_value = object["value"].getInt(-1);
}

void MachineC::toObject(UObject& object) const
{
	writeCommonProperties(object);
	object["value"] = m_value;
}

std::string MachineC::toString() const
{
	std::ostringstream s;
	s << Machine::toString() << ", value: " << m_value;
	return s.str();
}

void MachineD::fromObject(const UObject& object)
{
	readCommonProperties(object);
	m_isWorking = object["isWorking"].getInt(-1);
}

void MachineD::toObject(UObject& object) const
{
	writeCommonProperties(object);
	object["isWorking"] = m_isWorking;
}

std::string MachineD::toString() const
{
	std::ostringstream s;
	s << Machine::toString() << ", isWorking: " << (m_isWorking ? "true": "false");
	return s.str();
}

void Flotte::toObject(UObject& object) const
{
	object.setClass("flotte");
	UArray a;
	for (const auto& v : *this)
	{
		UObject o;
		v.toObject(o);
		a << o;
	}
	object["vehicules"] = a;
}

std::string Flotte::toString() const
{
	std::ostringstream s;
	s << "Flotte:" << std::endl;
	for (const auto& v : *this)
	{
		s << "* " << v.toString();
	}
	return s.str();
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
		push_back(VehiculefromString(v.getClass()));
		back().fromObject(v);
	}
}

SDT::SDT(const Machine& machine)
{
	switch (machine.getType())
	{
	case E_MachineType::C: m_ptr = std::make_unique<MachineC>(dynamic_cast<const MachineC&>(machine)); break;
	case E_MachineType::D: m_ptr = std::make_unique<MachineD>(dynamic_cast<const MachineD&>(machine)); break;
	default: m_ptr = nullptr;
	}
}
SDT::SDT(E_MachineType type) : m_ptr(Machine::build(type)) {}

E_MachineType SDT::getType() const
{
	return m_ptr->getType();
}

std::string SDT::getName() const
{
	return m_ptr->getName();
}

void SDT::toObject(uio::UObject& object) const
{
	m_ptr->toObject(object);
}

void SDT::fromObject(const uio::UObject& object)
{
	m_ptr->fromObject(object);
}

const Machine& SDT::getMachine() const
{
	return *m_ptr;
}

std::string SDT::toString() const
{
	return m_ptr->toString();
}

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
		push_back(MachineFromString(m.getClass()));
		back().fromObject(m);
	}
}

std::string SSP::getName() const
{
	return m_ptr->getName();
}

E_VehiculeType SSP::getType() const
{
	return m_ptr->getType();
}

std::string SSP::toString() const
{
	std::ostringstream s;
	s << "Véhicule: " << std::endl << " " << m_ptr->toString() << std::endl;
	s << "Machines: " << std::endl;
	for (const auto& m : *this)
	{
		s << " - " << m.toString() << std::endl;
	}
	return s.str();
}

void Named::fromObject(const UObject& object)
{
	m_name = object["name"].getString("undefined");
}

void Named::toObject(UObject& object) const
{
	object["name"] = m_name;
}

std::string Named::toString() const
{
	std::ostringstream s;
	s << "name: " << m_name;
	return s.str();
}
