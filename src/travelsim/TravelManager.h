
#ifndef TRAVEL_MANAGER_H
#define TRAVEL_MANAGER_H

#include "Location.h"
#include "Segment.h"
#include "Vehicle.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

using std::cout;
using std::endl;
using std::unordered_map;

//=======================================================
// Common functions
//=======================================================

template<typename K, typename V>
bool isKeyPresent(unordered_map<K,V> map, K key) {
	return (map.find(key) != map.end());
}

//=======================================================

//=======================================================
// SegmentTracker class
//=======================================================

class TravelManager;
class SegmentTracker : public Segment::Notifiee {
public:

	static Ptr<SegmentTracker> instanceNew() {
		return new SegmentTracker();
	}

	virtual void onSource(const Ptr<Location>& prevSource) { 
		if (prevSource != null) {
			prevSource->sourceSegmentDel(notifier());
		}

		auto currSource = notifier()->source();
		currSource->sourceSegmentIs(notifier());
	}

	virtual void onDestination(const Ptr<Location>& prevDestination) { 
		if (prevDestination != null) {
			prevDestination->destinationSegmentDel(notifier());
		}

		auto currDestination = notifier()->destination();
		currDestination->destinationSegmentIs(notifier());
	}

private:

	Ptr<TravelManager> travelManager_;
};

//=======================================================

//=======================================================
// TravelManager class
//=======================================================

class TravelManager : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<TravelManager> {
	public:

		void notifierIs(const Ptr<TravelManager>& travelManager) {
			connect(travelManager, this);
		}

		/* Notification that a new Residence has been instantiated */
		virtual void onResidenceNew(const Ptr<Residence>& residence) { }

		/* Notification that a new Airport has been instantiated */
		virtual void onAirportNew(const Ptr<Airport>& airport) { }

		/* Notification that a new Flight has been instantiated */
		virtual void onFlightNew(const Ptr<Flight>& flight) { }

		/* Notification that a new Road has been instantiated */
		virtual void onRoadNew(const Ptr<Road>& road) { }

		/* Notification that a new Airplane has been instantiated */
		virtual void onAirplaneNew(const Ptr<Airplane>& airplane) { }

		/* Notification that a new Car has been instantiated */
		virtual void onCarNew(const Ptr<Car>& car) { }

		/* Notification that a Location has been deleted */
		virtual void onLocationDel(const Ptr<Location>& location) { }

		/* Notification that a Segment has been deleted */
		virtual void onSegmentDel(const Ptr<Segment>& segment) { }

		/* Notification that a Vehicle has been deleted */
		virtual void onVehicleDel(const Ptr<Vehicle>& vehicle) { }
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

	Ptr<Airport> airportNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto airport = Airport::instanceNew(name);
		locationMap_.insert(LocationMap::value_type(name, airport));

		post(this, &Notifiee::onAirportNew, airport);

		return airport;
	}

	Ptr<Residence> residenceNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto residence = Residence::instanceNew(name);
		locationMap_.insert(LocationMap::value_type(name, residence));

		post(this, &Notifiee::onResidenceNew, residence);

		return residence;
	}

  	Ptr<Flight> flightNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto flight = Flight::instanceNew(name);
		segmentMap_.insert(SegmentMap::value_type(name, flight));
		
		addSegmentTracker(flight);

		post(this, &Notifiee::onFlightNew, flight);

		return flight;
	}

  	Ptr<Road> roadNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto road = Road::instanceNew(name);
		segmentMap_.insert(SegmentMap::value_type(name, road));

		addSegmentTracker(road);

		post(this, &Notifiee::onRoadNew, road);

		return road;
	}

	Ptr<Airplane> airplaneNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto airplane = Airplane::instanceNew(name);
		vehicleMap_.insert(VehicleMap::value_type(name, airplane));

		post(this, &Notifiee::onAirplaneNew, airplane);

		return airplane;
	}

	Ptr<Car> carNew(const string& name) {
		if (isNameInUse(name)) {
			throw fwk::NameInUseException(name);
		}

		const auto car = Car::instanceNew(name);
		vehicleMap_.insert(VehicleMap::value_type(name, car));

		post(this, &Notifiee::onCarNew, car);

		return car;
	}

	Ptr<Location> locationDel(const string& name) {
		auto iter = locationMap_.find(name);
		if (iter == locationMap_.end()) {
			return null;
		}

		const auto location = iter->second;
		locationMap_.erase(iter);

		post(this, &Notifiee::onLocationDel, location);

		return location;
	}

	Ptr<Segment> segmentDel(const string& name) {
		auto iter = segmentMap_.find(name);
		if (iter == segmentMap_.end()) {
			return null;
		}

		const auto segment = iter->second;
		segmentMap_.erase(iter);

		post(this, &Notifiee::onSegmentDel, segment);

		return segment;
	}

	Ptr<Vehicle> vehicleDel(const string& name) {
		auto iter = vehicleMap_.find(name);
		if (iter == vehicleMap_.end()) {
			return null;
		}

		const auto vehicle = iter->second;
		vehicleMap_.erase(iter);

		post(this, &Notifiee::onVehicleDel, vehicle);

		return vehicle;
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	TravelManager(const TravelManager&) = delete;

	void operator =(const TravelManager&) = delete;
	void operator ==(const TravelManager&) = delete;

protected:

	typedef unordered_map< string, Ptr<Location> > LocationMap;
	typedef unordered_map< string, Ptr<Segment> > SegmentMap;
	typedef unordered_map< string, Ptr<SegmentTracker> > SegmentTrackerMap;
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

	void addSegmentTracker(const Ptr<Segment>& segment) {
		const auto segmentTracker = SegmentTracker::instanceNew();
		segmentTracker->notifierIs(segment);
		segmentTrackerMap_.insert(SegmentTrackerMap::value_type(segment->name(), segmentTracker));
	}

	LocationMap locationMap_;
	SegmentMap segmentMap_;
	SegmentTrackerMap segmentTrackerMap_;
	VehicleMap vehicleMap_;
};

#endif