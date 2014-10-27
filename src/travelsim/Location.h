
#ifndef LOCATION_H
#define LOCATION_H

#include <algorithm>
#include <vector>

#include "Segment.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

using std::find;
using std::vector;

// TODO: Need to define Notifiee type

class Location : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<Location> {
	public:
		void notifierIs(const Ptr<Location>& location) {
			connect(location, this);
		}

		/* Notification that this location has been set as the 'source' of a segment */
		virtual void onSegmentNew(const Ptr<Segment>& segment) { };

		/* Notification that this location has been removed/unset as the 'source' of a segment */
		virtual void onSegmentDel(const Ptr<Segment>& segment) { };
	};

protected:

	typedef vector< Ptr<Segment> > SegmentVector;
	typedef SegmentVector::const_iterator const_iterator;
	typedef unsigned int SegmentId; // TODO: Should we use Ordinal type for this? To distinguish it from, say, length, etc.?
	typedef std::list<Notifiee*> NotifieeList;

public:

	static Ptr<Location> instanceNew(const string& name) {
		return new Location(name);
	}

	Ptr<Segment> segment(const U32 id) const {
		if (id < segments_.size()) {
			return segments_[id];
		}

		return null;
	}

	const_iterator segmentIter() const {
		return segments_.cbegin();
	}

	const_iterator segmentIterEnd() const {
		return segments_.cend();
	}

	// TODO: Is it correct to expose a collection attribute? Or should we just have iter and iterEnd attributes?
	/*
	const SegmentVector& segments() const {
		return segments_;
	}
	*/

	unsigned int segmentCount() const {
		return segments_.size();
	}

	void segmentIs(const Ptr<Segment> segment) {
		if (find (segments_.begin(), segments_.end(), segment) == segments_.end()) {
			segments_.push_back(segment); 
		}
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	Location(const Location&) = delete;

	void operator =(const Location&) = delete;
	void operator ==(const Location&) = delete;

protected:

	NotifieeList notifiees_;

	explicit Location(const string& name) :
		NamedInterface(name),
		segments_(0)
	{
		// Nothing to do
	}

	~Location() {
		//TODO: Do we need to free the Segment pointers in segments_. I don't think so.
	}

private:
	
	/* Segments for which the Location object is the 'source' */
	SegmentVector segments_;
};

class Airport : public Location {
public:

	/* Return a new Airport instance with the given name */
	static Ptr<Airport> instanceNew(const string& name) {
		return new Airport(name);
	}

protected:

	explicit Airport(const string& name):
		Location(name)
	{
		// Nothing to do
	}
};

class Residence : public Location {
public:

	/* Return a new Residence instance with the given name */
	static Ptr<Residence> instanceNew(const string& name) {
		return new Residence(name);
	}

protected:

	explicit Residence(const string& name):
		Location(name)
	{
		// Nothing to do
	}
};

#endif