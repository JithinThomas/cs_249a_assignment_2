
#ifndef VEHICLE_H
#define VEHICLE_H

#include "CommonLib.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::Ordinal;
using fwk::NotifierLib::post;
using fwk::Ptr;

class Capacity {};
class VehicleCapacity : public Ordinal<Capacity, int> {
public:

	VehicleCapacity(const int value = 0) :
		Ordinal(value)
	{
		if (value < 0) {
			throw fwk::RangeException("VehicleCapacity cannot be negative ('" + std::to_string(value) + "')");
		}
	}

	VehicleCapacity(const Ordinal<Capacity, int>& c) :
		Ordinal(c.value()) 
	{
		// Nothing else to do
	}

	VehicleCapacity(const VehicleCapacity& c):
		Ordinal(c.value())
	{
		// Nothing else to do
	}	
};

class Speed {};
class MilesPerHour : public Ordinal<Speed, int> {
public:

	MilesPerHour(const int value = 0) :
		Ordinal(value)
	{
		if (value < 0) {
			throw fwk::RangeException("MilesPerHour cannot be negative ('" + std::to_string(value) + "')");
		}
	}

	MilesPerHour(const Ordinal<Speed, int>& c) :
		Ordinal(c.value()) 
	{
		// Nothing else to do
	}

	MilesPerHour(const MilesPerHour& m):
		Ordinal(m.value())
	{
		// Nothing else to do
	}	
};

class Vehicle;

class VehicleCost : public Ordinal<Vehicle, double> {
public:

	static constexpr double tolerance = 1e-4;

	VehicleCost(const double value = 0) :
		Ordinal(value)
	{
		if (value < 0) {
			throw fwk::RangeException("VehicleCost cannot be negative");
		}
	}

	VehicleCost(const Ordinal<Vehicle, int>& c) :
		Ordinal(c.value()) 
	{
		// Nothing else to do
	}

	VehicleCost(const VehicleCost& cost) :
		Ordinal(cost.value_)
	{
		// Nothing else to do.
	}

	double value() const {
		return value_;
	}

	void valueIs(const double value) {
		value_ = value;
	}

	void valueIs(const VehicleCost& cost) {
		value_ = cost.value_;
	}

	/** Test for equality using a builtin tolerance. */
	virtual bool operator ==(const VehicleCost& cost) {
		return (value_ < cost.value_ + tolerance) && (value_ > cost.value_ - tolerance);
	}

	/** Test for inequality using a builtin tolerance. */
    virtual bool operator !=(const VehicleCost& cost) const {
        return (value_ >= cost.value_ + tolerance) || (value_ <= cost.value_ - tolerance);
    }
};

class Vehicle : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<Vehicle> {
	public:
		void notifierIs(const Ptr<Vehicle>& vehicle) {
			connect(vehicle, this);
		}

		/* Notification that the vehicle's capacity has been modified */
		virtual void onCapacity() { }

		/* Notification that the vehicle's speed has been modified */
		virtual void onSpeed() { }

		/* Notification that the vehicle's cost has been modified */
		virtual void onCost() { }
	};

	static Ptr<Vehicle> instanceNew(const string& name) {
		return new Vehicle(name);
	}

protected:

	typedef std::list<Notifiee*> NotifieeList;

public:

	VehicleCapacity capacity() const {
		return capacity_;
	}

	MilesPerHour speed() const {
		return speed_;
	}

	VehicleCost cost() const {
		return cost_;
	}

	void capacityIs(const VehicleCapacity& capacity) {
		if (capacity_ != capacity) {
			capacity_ = capacity;
			post(this, &Notifiee::onCapacity);
		}
	}

	void speedIs(const MilesPerHour& speed) {
		if (speed_ != speed) {
			speed_ = speed;
			post(this, &Notifiee::onSpeed);
		}
	}

	void costIs(const VehicleCost& cost) {
		if (cost_ != cost) {
			cost_ = cost;
			post(this, &Notifiee::onCost);
		}
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	Vehicle(const Vehicle&) = delete;

	void operator =(const Vehicle&) = delete;
	void operator ==(const Vehicle&) = delete;

protected:

	static const VehicleCapacity defaultCapacity;
	static const MilesPerHour defaultSpeed;
	static const VehicleCost defaultCost;

	NotifieeList notifiees_;

	Vehicle(const string& name) :
		NamedInterface(name),
		capacity_(defaultCapacity),
		speed_(defaultSpeed),
		cost_(defaultCost)
	{
		// Nothing to do
	}

	Vehicle(const string& name,
			const VehicleCapacity& capacity,
			const MilesPerHour& speed,
			const VehicleCost& cost) :
		NamedInterface(name),
		capacity_(capacity),
		speed_(speed),
		cost_(cost)
	{
		// Nothing to do
	}

	~Vehicle() {}

private:
	VehicleCapacity capacity_;
	MilesPerHour speed_;
	VehicleCost cost_;
};

const VehicleCapacity Vehicle::defaultCapacity = VehicleCapacity(0);
const VehicleCost Vehicle::defaultCost = VehicleCost(0);
const MilesPerHour Vehicle::defaultSpeed = MilesPerHour(0);

class Airplane : public Vehicle {
public:
	static Ptr<Airplane> instanceNew(const string& name) {
		return new Airplane(name);
	}

protected:

	Airplane(const string& name) :
		Vehicle(name)
	{
		// Nothing to do
	}

	Airplane(const string& name,
			 const VehicleCapacity& capacity,
			 const MilesPerHour& speed,
			 const VehicleCost& cost) :
		Vehicle(name, capacity, speed, cost)
	{
		// Nothing to do
	}
};

class Car : public Vehicle {
public:
	static Ptr<Car> instanceNew(const string& name) {
		return new Car(name);
	}

protected:
	
	Car(const string& name) :
		Vehicle(name)
	{
		// Nothing to do
	}

	Car(const string& name,
		const VehicleCapacity& capacity,
		const MilesPerHour& speed,
		const VehicleCost& cost) :
		Vehicle(name, capacity, speed, cost)
	{
		// Nothing to do
	}
};

#endif