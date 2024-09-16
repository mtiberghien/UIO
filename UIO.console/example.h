#pragma once
#include <string>
#include <memory>
#include <vector>
#include "UIO_All.h"
#include "IUSerializable.h"

using namespace uio;

enum E_VehiculeType{ A, B};
enum E_MachineType {C, D};

class Machine : public uio::IUSerializable
{
public:
	Machine() {}
	virtual ~Machine() {}
	std::string getName() const { return m_name; }
	void setName(const std::string& name) { m_name = name; }
	virtual E_MachineType getType() const = 0;
	static std::unique_ptr<Machine> build(E_MachineType type);
	// Inherited via IUSerializable
	virtual void toObject(uio::UObject& object) const override;
	virtual void fromObject(const uio::UObject& object) override;
protected:
	void writeCommonProperties(UObject& object) const;
	void readCommonProperties(const UObject& object);
	
private:
	std::string m_name;
};

class MachineC: public Machine
{
public:
	MachineC() {}
	MachineC(const std::string& name, int value = 0) :m_value(value)
	{
		setName(name);
	}
	E_MachineType getType() const override { return E_MachineType::C; }
	int getValue() const { return m_value; }
	void setValue(int value) { m_value = value; }
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
private:
	int m_value{ 0 };
};

class MachineD : public Machine
{
public:
	MachineD() {}
	MachineD(const std::string& name, bool isWorking = false): m_isWorking(isWorking)
	{
		setName(name);
	}
	E_MachineType getType() const override { return E_MachineType::D; }
	bool getIsWorking() const { return m_isWorking; }
	void setIsWorking(bool isWorking) { m_isWorking = isWorking; }
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
private:
	bool m_isWorking{ false };
};



class Vehicule: public std::vector<std::unique_ptr<Machine>>, public IUSerializable
{
public:
	Vehicule() {}
	Vehicule(const std::string& name) : m_name(name) {}
	virtual ~Vehicule() {}
	std::string getName() const { return m_name; }
	void setName(const std::string& name) { m_name = name; }
	virtual E_VehiculeType getType() const = 0;
	static std::unique_ptr<Vehicule> build(E_VehiculeType type);
	// Inherited via IUSerializable
	virtual void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;

private:
	std::string m_name;
};

class VehiculeA : public Vehicule
{
public:
	VehiculeA() {}
	VehiculeA(const std::string& name) 
	{
		setName(name);
	}
	E_VehiculeType getType() const override { return E_VehiculeType::A; }
	void toObject(UObject& object) const override
	{
		Vehicule::toObject(object);
		object.setName("A");
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
		object.setName("B");
	}
};

class Flotte : public std::vector<std::unique_ptr<Vehicule>>, public IUSerializable
{
public:
	// Inherited via IUSerializable
	void toObject(UObject& object) const override;
	void fromObject(const UObject& object) override;
};



