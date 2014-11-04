
#ifndef TRAVEL_MANAGER_H
#define TRAVEL_MANAGER_H

#include "CommonLib.h"
#include "Conn.h"
#include "Flight.h"
#include "Location.h"
#include "Segment.h"
#include "Vehicle.h"
//#include "TravelManagerTracker.h"

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

class TravelManagerTracker;

class TravelManager : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<TravelManager> {
	public:

		void notifierIs(const Ptr<TravelManager>& travelManager) {
			connect(travelManager, this);
		}

		// TODO: Just have a notification for LocationNew, SegmentNew and VehicleNew
		//       The implementation class can check the dynamic type of the new object and act accordingly

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

	typedef unordered_map< string, Ptr<Location> > LocationMap;
	typedef unordered_map< string, Ptr<Segment> > SegmentMap;
	typedef unordered_map< string, Ptr<SegmentTracker> > SegmentTrackerMap;
	typedef unordered_map< string, Ptr<Vehicle> > VehicleMap;

	typedef LocationMap::const_iterator locationConstIter;
	typedef SegmentMap::const_iterator segmentConstIter;
	typedef VehicleMap::const_iterator vehicleConstIter;

	typedef LocationMap::iterator locationIterator;
	typedef SegmentMap::iterator segmentIterator;
	typedef VehicleMap::iterator vehicleIterator;

	typedef std::list<Notifiee*> NotifieeList;

public:

	/* Location accessors by name */
	Ptr<Location> location(const string& name) const {
		return getEntityFromMap<Location>(locationMap_, name);
	}

	Ptr<Airport> airport(const string& name) const {
		return findLocationOfSpecificType<Airport>(name);
	}

	Ptr<Residence> residence(const string& name) const {
		return findLocationOfSpecificType<Residence>(name);
	}

	/* Segment accessors by name */
	Ptr<Segment> segment(const string& name) const {
		return getEntityFromMap<Segment>(segmentMap_, name);
	}

	Ptr<Flight> flight(const string& name) const {
		return findSegmentOfSpecificType<Flight>(name);
	}

	Ptr<Road> road(const string& name) const {
		return findSegmentOfSpecificType<Road>(name);
	}

	/* Vehicle accessors by name */
	Ptr<Vehicle> vehicle(const string& name) const {
		return getEntityFromMap<Vehicle>(vehicleMap_, name);
	}

	Ptr<Airplane> airplane(const string& name) const {
		return findVehicleOfSpecificType<Airplane>(name);
	}

	Ptr<Car> car(const string& name) const {
		return findVehicleOfSpecificType<Car>(name);
	}

	locationConstIter locationIter() {
		return locationMap_.cbegin();
	}

	locationConstIter locationIterEnd() {
		return locationMap_.cend();
	}

	segmentConstIter segmentIter() {
		return segmentMap_.cbegin();
	}

	segmentConstIter segmentIterEnd() {
		return segmentMap_.cend();
	}

	vehicleConstIter vehicleIter() {
		return vehicleMap_.cbegin();
	}

	vehicleConstIter vehicleIterEnd() {
		return vehicleMap_.cend();
	}

	Ptr<Airport> airportNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
		}

		const auto airport = Airport::instanceNew(name);
		locationMap_.insert(LocationMap::value_type(name, airport));

		post(this, &Notifiee::onAirportNew, airport);

		return airport;
	}

	Ptr<Residence> residenceNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
		}

		const auto residence = Residence::instanceNew(name);
		locationMap_.insert(LocationMap::value_type(name, residence));

		post(this, &Notifiee::onResidenceNew, residence);

		return residence;
	}

  	Ptr<Flight> flightNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
		}

		const auto flight = Flight::instanceNew(name);
		segmentMap_.insert(SegmentMap::value_type(name, flight));
		
		addSegmentTracker(flight);

		post(this, &Notifiee::onFlightNew, flight);

		return flight;
	}

  	Ptr<Road> roadNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
		}

		const auto road = Road::instanceNew(name);
		segmentMap_.insert(SegmentMap::value_type(name, road));

		addSegmentTracker(road);

		post(this, &Notifiee::onRoadNew, road);

		return road;
	}

	Ptr<Airplane> airplaneNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
		}

		const auto airplane = Airplane::instanceNew(name);
		vehicleMap_.insert(VehicleMap::value_type(name, airplane));

		post(this, &Notifiee::onAirplaneNew, airplane);

		return airplane;
	}

	Ptr<Car> carNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;
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
		locationDel(iter);
		
		return location;
	}

	locationIterator locationDel(locationConstIter iter) {
		const auto location = iter->second;
		auto next = locationMap_.erase(iter);

		// Nullify the 'source' attribute of all segments for which this location was the source
		for (auto i = 0; i < location->sourceSegmentCount(); i++) {
			location->sourceSegment(i)->sourceDel();
		}

		// Nullify the 'destintaion' attribute of all segments for which this location was the destination
		for (auto i = 0; i < location->destinationSegmentCount(); i++) {
			location->destinationSegment(i)->destinationDel();
		}

		post(this, &Notifiee::onLocationDel, location);

		return next;
	}

	segmentIterator segmentDel(segmentConstIter iter) {
		const auto segment = iter->second;
		auto next = segmentMap_.erase(iter);

		post(this, &Notifiee::onSegmentDel, segment);

		return next;
	}

	Ptr<Segment> segmentDel(const string& name) {
		auto iter = segmentMap_.find(name);
		if (iter == segmentMap_.end()) {
			return null;
		}

		const auto segment = iter->second;
		segmentDel(iter);

		return segment;
	}

	vehicleIterator vehicleDel(vehicleConstIter iter) {
		const auto vehicle = iter->second;
		auto next = vehicleMap_.erase(iter);

		post(this, &Notifiee::onVehicleDel, vehicle);

		return next;
	}

	Ptr<Vehicle> vehicleDel(const string& name) {
		auto iter = vehicleMap_.find(name);
		if (iter == vehicleMap_.end()) {
			return null;
		}

		const auto vehicle = iter->second;
		vehicleDel(iter);

		return vehicle;
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	TravelManager(const TravelManager&) = delete;

	void operator =(const TravelManager&) = delete;
	void operator ==(const TravelManager&) = delete;

protected:

	NotifieeList notifiees_;

	explicit TravelManager(const string& name) :
		NamedInterface(name)
	{
		// Nothing to do
	}

private:

	bool isNameInUse(const string& name) {
		return (isKeyPresent(locationMap_, name) ||
				isKeyPresent(segmentMap_, name)  ||
				isKeyPresent(vehicleMap_, name));
	}

	template<class T>
	Ptr<T> findLocationOfSpecificType(const string& name) const {
		Ptr<Location> loc = location(name);
		if (loc != null) {
			auto ptr = dynamic_cast<T*>(loc.ptr());
			if (ptr != null) {
				return ptr;
			} 
		}

		return null;
	}

	template<class T>
	Ptr<T> findSegmentOfSpecificType(const string& name) const {
		Ptr<Segment> seg = segment(name);
		if (seg != null) {
			auto ptr = dynamic_cast<T*>(seg.ptr());
			if (ptr != null) {
				return ptr;
			} 
		}

		return null;
	}

	template<class T>
	Ptr<T> findVehicleOfSpecificType(const string& name) const {
		Ptr<Vehicle> v = vehicle(name);
		if (v != null) {
			auto ptr = dynamic_cast<T*>(v.ptr());
			if (ptr != null) {
				return ptr;
			} 
		}

		return null;
	}

	template<class T>
	Ptr<T> getEntityFromMap(const unordered_map<string, Ptr<T>>& entityMap, const string& name) const {
		const auto it = entityMap.find(name);
		if (it != entityMap.end()) {
			return it->second;
		}

		return null;
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



	/*
	Ptr<TravelManagerTracker> statsNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;	
		}

		const auto stats = TravelManagerTracker::instanceNew(name);
		statsMap_.insert(StatsMap::value_type(name, stats));

		post(this, &Notifiee::onStatsNew, stats);

		return stats;
	}

	Ptr<Conn> connNew(const string& name) {
		if (isNameInUse(name)) {
			logError(WARNING, "An instance with the given name '" + name + "' already exists. Skipping command.");
			return null;	
		}

		const auto conn = Conn::instanceNew(name);
		connMap_.insert(ConnMap::value_type(name, conn));

		post(this, &Notifiee::onConnNew, conn);

		return conn;
	}
	*/

#endif