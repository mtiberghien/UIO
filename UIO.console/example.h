#pragma once
#include <string>
#include <memory>
#include <vector>
#include "UIO_All.h"
#include "IUSerializable.h"

using namespace uio;

enum E_VehiculeType{ A, B};
enum E_MachineType {C, D};

static std::string toString(E_VehiculeType type)
{
	switch (type)
	{
	case E_VehiculeType::A: return "A";
	case E_VehiculeType::B: return "B";
	default: return "Inconnu";
	}
}

static std::string toString(E_MachineType type)
{
	switch (type)
	{
	case E_MachineType::C: return "C";
	case E_MachineType::D: return "D";
	default: return "Inconnu";
	}
}

class IObject
{
public:
	virtual ~IObject() {}
	virtual std::string toString() const = 0;
};

class INamed
{
public:
	virtual ~INamed() {}
	virtual std::string getName() const = 0;
};

class Named: public virtual INamed, public virtual IUSerializable, public virtual IObject
{
public:
	Named(std::string name) : m_name(name) {}
	virtual ~Named() {}
	std::string getName() const override { return m_name; }
	void setName(const std::string& name) { m_name = name; }
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	std::string toString() const override;
private:
	std::string m_name{ "" };
};

class IMachine: public virtual IUSerializable, public virtual IObject
{
public:
	virtual ~IMachine() {}
	virtual E_MachineType getType() const = 0;
};

class Machine;

class SDT : public virtual IMachine, public virtual INamed
{
public:
	SDT(const Machine& machine);
	SDT(E_MachineType type);
	E_MachineType getType() const override;
	std::string getName() const override;
	virtual void toObject(uio::UObject& object) const override;
	virtual void fromObject(const uio::UObject& object) override;
	const Machine& getMachine() const;
	virtual std::string toString() const override;
private:
	std::unique_ptr<Machine> m_ptr;
};

class Machine : public Named, public virtual IMachine
{
public:
	Machine(): Named("Machine") {}
	Machine(std::string name) : Named(name) {}
	Machine(const Machine& machine) = default;
	virtual ~Machine() {}
	static std::unique_ptr<Machine> build(E_MachineType type);
	virtual void toObject(uio::UObject& object) const override;
	virtual void fromObject(const uio::UObject& object) override;
	virtual std::string toString() const override;
protected:
	void writeCommonProperties(UObject& object) const;
	void readCommonProperties(const UObject& object);
	
};

class MachineC: public Machine
{
public:
	MachineC() {}
	MachineC(const std::string& name, int value = 0) : Machine(name), m_value(value){}
	E_MachineType getType() const override { return E_MachineType::C; }
	int getValue() const { return m_value; }
	void setValue(int value) { m_value = value; }
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	std::string toString() const override;
private:
	int m_value{ 0 };
};

class MachineD : public Machine
{
public:
	MachineD() {}
	MachineD(const std::string& name, bool isWorking = false) : Machine(name), m_isWorking(isWorking) {}
	E_MachineType getType() const override { return E_MachineType::D; }
	bool getIsWorking() const { return m_isWorking; }
	void setIsWorking(bool isWorking) { m_isWorking = isWorking; }
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	std::string toString() const override;
private:
	bool m_isWorking{ false };
};

class IVehicule : public virtual IUSerializable, public virtual IObject
{
public:
	virtual ~IVehicule() {}
	virtual E_VehiculeType getType() const = 0;
};

class Vehicule;

class SSP : public std::vector<SDT>, public virtual IVehicule, public virtual INamed
{
public:
	SSP(E_VehiculeType type);
	SSP(const Vehicule& vehicule);
	std::string getName() const override;
	E_VehiculeType getType() const override;
	virtual void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	std::string toString() const override;
private:
	std::unique_ptr<Vehicule> m_ptr;
};

class Vehicule: public Named, public virtual IVehicule
{
public:
	Vehicule(): Named("V�hicule") {}
	Vehicule(const Vehicule& ref) = default;
	Vehicule(const std::string& name) : Named(name) {}
	virtual ~Vehicule() {}
	virtual E_VehiculeType getType() const = 0;
	static std::unique_ptr<Vehicule> build(E_VehiculeType type);
	virtual void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	virtual std::string toString() const override;
};

class VehiculeA : public Vehicule
{
public:
	VehiculeA() {}
	VehiculeA(const std::string& name) : Vehicule(name) {}
	E_VehiculeType getType() const override { return E_VehiculeType::A; }
	void toObject(UObject& object) const override
	{
		Vehicule::toObject(object);
	}
};

class VehiculeB : public Vehicule
{
public:
	VehiculeB() {}
	VehiculeB(const std::string& name) : Vehicule(name) {}
	E_VehiculeType getType() const override { return E_VehiculeType::B; }
	void toObject(UObject& object) const override
	{
		Vehicule::toObject(object);
	}
};

class Flotte : public std::vector<SSP>, public virtual IUSerializable, public virtual IObject
{
public:
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
	std::string toString() const override;
};



