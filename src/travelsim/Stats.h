
#ifndef STATS_H
#define STATS_H

#include <Location.h>
#include <Segment.h>
#include <Vehicle.h>

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

class Stats : public NamedInterface {
public:

	static Ptr<Stats> instanceNew(const string& name) {
		return new Stats(name);
	}

	Stats(const Stats&) = delete;

	void operator =(const Stats&) = delete;

protected:

	Stats(const string& name) :
		NamedInterface(name)
	{

	}

private:
	TravelNetworkTracker travelNetworkTracker_;

};

#endif