
#ifndef CONN_H
#define CONN_H

#include <set>

#include "Location.h"
#include "Segment.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::Nominal;
using fwk::NotifierLib::post;
using fwk::Ordinal;
using fwk::Ptr;
using fwk::PtrInterface;

using std::set;
using std::to_string;

class Path : public PtrInterface {
public:

	static Ptr<Path> instanceNew() {
		return new Path();
	}

	static Ptr<Path> instanceNew(const Ptr<Path>& p) {
		return new Path(p);
	}

	void segmentIs(const Ptr<Segment> segment) {
		segments_.push_back(segment);
		length_ = length_ + segment->length();
	}

	vector< Ptr<Segment> > segments() const {
		return segments_;
	}

	SegmentLength length() const {
		return length_;
	}

	string toString() const {
		string str = "";
		auto segmentCount = segments_.size();
		for (auto i = 0; i < segmentCount; i++) {
			auto seg = segments_[i];
			str += seg->source()->name() + "(" + seg->name() + ":" + to_string(seg->length().value()) + ") ";
		}

		if (segmentCount > 0) {
			str += segments_[segmentCount - 1]->destination()->name();
		}

		return str;
	}

protected:

	Path():
		length_(0)
	{
		// Nothing else to do
	}

	Path(const Ptr<Path>& p) :
		segments_(p->segments()),
		length_(p->length())
	{
		// Nothing else to do
	}

private:

	vector< Ptr<Segment> > segments_;
	SegmentLength length_;
};

class Conn : public NamedInterface {
public:

	static Ptr<Conn> instanceNew(const string& name) {
		return new Conn(name);
	}

protected:

	typedef vector< Ptr<Path> > PathVector;

public:
	const PathVector paths(const Ptr<Location>& location, const SegmentLength& maxLength) const {
		set<string> locationsVisited;
		locationsVisited.insert(location->name());

		return getPathsFromLoc(location, Path::instanceNew(), maxLength, locationsVisited);
	}

	Conn(const Conn&) = delete;

	void operator =(const Conn&) = delete;
	void operator ==(const Conn&) = delete;

protected:

	Conn(const string& name):
		NamedInterface(name)
	{
		// Nothing else to do
	}

	~Conn() { }

private:

	PathVector getPathsFromLoc(const Ptr<Location>& location,
							   const Ptr<Path>& pathFromStartLocation,
							   const SegmentLength& maxLength, 
							   set<string> locationsVisited
							  ) const {
		PathVector validPaths;

		for (auto i = 0; i < location->sourceSegmentCount(); i++) {		
			auto segment = location->sourceSegment(i);
			SegmentLength totalLengthOfPath = pathFromStartLocation->length() + segment->length();
			auto destination = segment->destination();

			if ((totalLengthOfPath <= maxLength) && (locationsVisited.find(destination->name()) == locationsVisited.end())) {
				auto p = Path::instanceNew(pathFromStartLocation);
				p->segmentIs(segment);
				validPaths.push_back(p);

				locationsVisited.insert(destination->name());
				auto otherPaths = getPathsFromLoc(destination, p, maxLength, locationsVisited);
				locationsVisited.erase(destination->name());

				for (auto p : otherPaths) {
					validPaths.push_back(p);
				}
			}
		}

		return validPaths;
	}
};


#endif