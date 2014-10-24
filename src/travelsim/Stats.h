
#ifndef STATS_H
#define STATS_H

#include <Location.h>
#include <Segment.h>
#include <Vehicle.h>
#include <TravelManager.h>
#include <TravelManagerTracker.h>

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

using std::cout;
using std::endl;

class TravelManager;
class TravelManagerTracker;

//class Stats : public TravelManager::Notifiee {
class Stats : public NamedInterface {
public:

	static Ptr<Stats> instanceNew(const string& name, const Ptr<TravelManager>& travelManager) {
		return new Stats(name, travelManager);
	}

	TravelManagerTracker tracker() const {
		return travelManagerTracker_;
	}

	Stats(const Stats&) = delete;

	void operator =(const Stats&) = delete;
	void operator ==(const Stats&) = delete;

protected:

	Stats(const string& name, const Ptr<TravelManager>& travelManager) :
		NamedInterface(name)
	{
		travelManager_ = travelManager;
		travelManagerTracker_.notifierIs(travelManager_);
	}

private:
	Ptr<TravelManager> travelManager_;
	TravelManagerTracker travelManagerTracker_;
};

#endif