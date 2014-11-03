
#ifndef TRAVEL_MANAGER_TRACKER_H
#define TRAVEL_MANAGER_TRACKER_H

#include "TravelManager.h"

// TODO: Implement the onLocationDel, etc. notification methods

class TravelManagerTracker : public TravelManager::Notifiee {
public:

	static Ptr<TravelManagerTracker> instanceNew(const string& name) {
		return new TravelManagerTracker(name);
	}

	unsigned int residenceCount() const {
		return residenceCount_;
	}

	unsigned int airportCount() const {
		return airportCount_;
	}

	unsigned int flightCount() const {
		return flightCount_;
	}

	unsigned int roadCount() const {
		return roadCount_;
	}

	unsigned int airplaneCount() const {
		return airplaneCount_;
	}

	unsigned int carCount() const {
		return carCount_;
	}

	void onResidenceNew(const Ptr<Residence>& residence) {
		residenceCount_++;
	}

	void onAirportNew(const Ptr<Airport>& airport) {
		airportCount_++;
	}

	void onFlightNew(const Ptr<Flight>& flight) {
		flightCount_++;
	}

	void onRoadNew(const Ptr<Road>& road) {
		roadCount_++;
	}

	void onAirplaneNew(const Ptr<Airplane>& airplane) {
		airplaneCount_++;
	}

	void onCarNew(const Ptr<Car>& car) {
		carCount_++;
	}

	const string& name() const {
		return name_;
	}

protected:

	explicit TravelManagerTracker(const string& name) :
		airplaneCount_(0),
		airportCount_(0),
		carCount_(0),
		flightCount_(0),
		residenceCount_(0),
		roadCount_(0),
		name_(name)
	{

	}

private:
	unsigned int airplaneCount_;
	unsigned int airportCount_;
	unsigned int carCount_;
	unsigned int flightCount_;
	unsigned int residenceCount_;
	unsigned int roadCount_;

	string name_;
};

#endif