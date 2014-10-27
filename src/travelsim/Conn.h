
#ifndef CONN_H
#define CONN_H

#include <set>

#include "TravelManager.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::Nominal;
using fwk::NotifierLib::post;
using fwk::Ordinal;
using fwk::Ptr;

using std::set;

class Conn : public NamedInterface {
public:

	static Ptr<Conn> instanceNew(const string& name) {
		return new Conn(name);
	}

protected:

	typedef Nominal<Conn, string> Path;
	typedef vector<Path> PathVector;

public:

	const Ptr<TravelManager>& travelManager() const {
		return travelManager_;
	}

	void travelManagerIs(const Ptr<TravelManager>& travelManager) {
		travelManager_ = travelManager;
	}

	const PathVector paths(const Ptr<Location>& location, const SegmentLength& maxLength) const {
		set<Ptr<Location>> locationsVisited;
		return getPathsFromLoc(location, 0, maxLength, locationsVisited);
	}

	Conn(const Conn&) = delete;

	void operator =(const Conn&) = delete;
	void operator ==(const Conn&) = delete;

protected:

	Conn(const string& name):
		NamedInterface(name)
	{
		travelManager_ = null;
	}

	~Conn() { }

private:

	PathVector getPathsFromLoc(const Ptr<Location>& location, 
							   const SegmentLength& lengthFromStartToLoc,
							   const SegmentLength& maxLength, 
							   set<Ptr<Location>> locationsVisited
							  ) const {
		PathVector paths;
		for (auto it = location->segmentIter(); it != location->segmentIterEnd(); it++) {
			auto segment = *it;
			SegmentLength totalLengthOfPath = lengthFromStartToLoc + segment->length();
			auto destination = segment->destination();

			if ((totalLengthOfPath <= maxLength) && (locationsVisited.find(destination) == locationsVisited.end())) {
				paths.push_back(location->name() + "(" + segment->name() + ":" +  + ")");

				locationsVisited.insert(destination);
				auto otherPaths = getPathsFromLoc(destination, totalLengthOfPath, maxLength, locationsVisited);
				locationsVisited.erase(destination);

				paths.insert(paths.end(), otherPaths.begin(), otherPaths.end());
			}
		}

		return paths;
	}

	Ptr<TravelManager> travelManager_;
};


#endif