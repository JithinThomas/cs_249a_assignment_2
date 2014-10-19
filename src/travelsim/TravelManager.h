
#ifndef TRAVEL_MANAGER_H
#define TRAVEL_MANAGER_H

#include "Location.h"
#include "Segment.h"
#include "Vehicle.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

using std::unordered_map;

//=======================================================
// Common functions
//=======================================================

template<typename K, typename V>
bool isKeyPresent(unordered_map<K,V> map, K key) {
	return (map.find(key) != map.end());
}

//=======================================================

class TravelManager : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<TravelManager> {
		void notifierIs(const Ptr<TravelManager>& travelManager) {
			connect(travelManager, this);
		}

		/* Notification that a new Location has been instantiated */
		virtual void onLocationNew(const Ptr<Location>& location) { }

		/* Notification that a new Flight has been instantiated */
		virtual void onFlightNew(const Ptr<Flight>& flight) { }

		/* Notification that a new Road has been instantiated */
		virtual void onRoadNew(const Ptr<Road>& road) { }

		/* Notification that a new Airplane has been instantiated */
		virtual void onAirplaneNew(const Ptr<Airplane>& airplane) { }

		/* Notification that a new Car has been instantiated */
		virtual void onCarNew(const Ptr<Car>& car) { }
	};

	static Ptr<TravelManager> instanceNew(const string& name) {
		return new TravelManager(name);
	}

protected:

	typedef std::list<Notifiee*> NotifieeList;

public:

	Ptr<Location> location(const string& name) const {
		const auto it = locationMap_.find(name);
		if (it != locationMap_.end()) {
			return it->second;
		}

		return null;
	}

	Ptr<Segment> segment(const string& name) const {
		const auto it = segmentMap_.find(name);
		if (it != segmentMap_.end()) {
			return it->second;
		}

		return null;
	}

	Ptr<Vehicle> vehicle(const string& name) const {
		const auto it = vehicleMap_.find(name);
		if (it != vehicleMap_.end()) {
			return it->second;
		}

		return null;
	}

	Ptr<Location> locationNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto location = Location::instanceNew(name);
		locationMap_.insert(LocationMap::value_type(name, location));

		return location;
	}

	Ptr<Flight> flightNew(const string& name,
						  const Ptr<Location>& source, 
						  const Ptr<Location>& destination,
						  const SegmentLength& length) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto flight = Flight::instanceNew(name, source, destination, length);
		segmentMap_.insert(SegmentMap::value_type(name, flight));

		return flight;
	}

	Ptr<Road> roadNew(const string& name,
					  const Ptr<Location>& source, 
					  const Ptr<Location>& destination,
					  const SegmentLength& length) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto road = Road::instanceNew(name, source, destination, length);
		segmentMap_.insert(SegmentMap::value_type(name, road));

		return road;
	}

	Ptr<Airplane> airplaneNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto airplane = Airplane::instanceNew(name);
		vehicleMap_.insert(VehicleMap::value_type(name, airplane));

		return airplane;
	}

	Ptr<Car> carNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto car = Car::instanceNew(name);
		vehicleMap_.insert(VehicleMap::value_type(name, car));

		return car;
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	TravelManager(const TravelManager&) = delete;

	void operator =(const TravelManager&) = delete;

protected:

	typedef unordered_map< string, Ptr<Location> > LocationMap;
	typedef unordered_map< string, Ptr<Segment> > SegmentMap;
	typedef unordered_map< string, Ptr<Vehicle> > VehicleMap;

	NotifieeList notifiees_;

	explicit TravelManager(const string& name) :
		NamedInterface(name)
	{
		// Nothing to do
	}

private:

	bool isNameInUse(const string& name) {
		return (isKeyPresent(locationMap_, name) ||
				isKeyPresent(segmentMap_, name) ||
				isKeyPresent(vehicleMap_, name));
	}

	LocationMap locationMap_;
	SegmentMap segmentMap_;
	VehicleMap vehicleMap_;
};

#endif