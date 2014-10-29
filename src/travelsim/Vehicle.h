
#ifndef VEHICLE_H
#define VEHICLE_H

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::Ordinal;
using fwk::NotifierLib::post;
using fwk::Ptr;

/*
class VehicleCap {};
typedef Ordinal<VehicleCap, unsigned int> VehicleCapacity;

class VehicleSp {};
typedef Ordinal<VehicleSp, unsigned int> VehicleSpeed;

class VehicleCo {};
typedef Ordinal<VehicleCo, unsigned int> VehicleCost;
*/

typedef unsigned int VehicleCapacity;
typedef unsigned int VehicleSpeed;
typedef unsigned int VehicleCost;

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

	VehicleSpeed speed() const {
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

	void speedIs(const VehicleSpeed& speed) {
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

	static const VehicleCapacity defaulCapacity = 0;
	static const VehicleSpeed defaulSpeed = 0;
	static const VehicleCost defaulCost = 0;

	NotifieeList notifiees_;

	Vehicle(const string& name) :
		NamedInterface(name),
		capacity_(defaulCapacity),
		speed_(defaulSpeed),
		cost_(defaulCost)
	{
		// Nothing to do
	}

	Vehicle(const string& name,
			const VehicleCapacity& capacity,
			const VehicleSpeed& speed,
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
	VehicleSpeed speed_;
	VehicleCost cost_;
};

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
			 const VehicleSpeed& speed,
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
		const VehicleSpeed& speed,
		const VehicleCost& cost) :
		Vehicle(name, capacity, speed, cost)
	{
		// Nothing to do
	}
};

#endif