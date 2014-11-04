
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
		virtual void onSourceSegmentNew(const Ptr<Segment>& segment) { };

		/* Notification that this location has been removed/unset as the 'source' of a segment */
		virtual void onSourceSegmentDel(const Ptr<Segment>& segment) { };

		/* Notification that this location has been set as the 'destination' of a segment */
		virtual void onDestinationSegmentNew(const Ptr<Segment>& segment) { };

		/* Notification that this location has been removed/unset as the 'destination' of a segment */
		virtual void onDestinationSegmentDel(const Ptr<Segment>& segment) { };
	};

protected:

	typedef vector< Ptr<Segment> > SegmentVector;
	typedef SegmentVector::const_iterator const_iterator;
	typedef SegmentVector::iterator iterator;
	//typedef unsigned int SegmentId; // TODO: Should we use Ordinal type for this? To distinguish it from, say, length, etc.?
	typedef std::list<Notifiee*> NotifieeList;

public:

	static Ptr<Location> instanceNew(const string& name) {
		return new Location(name);
	}

	Ptr<Segment> sourceSegment(const U32 id) const {
		return findSegment(sourceSegments_, id);
	}

	Ptr<Segment> destinationSegment(const U32 id) const {
		return findSegment(destinationSegments_, id);
	}

	const_iterator sourceSegmentIter() const {
		return sourceSegments_.cbegin();
	}

	const_iterator sourceSegmentIterEnd() const {
		return sourceSegments_.cend();
	}

	const_iterator destinationSegmentIter() const {
		return destinationSegments_.cbegin();
	}

	const_iterator destinationSegmentIterEnd() const {
		return destinationSegments_.cend();
	}

	unsigned int sourceSegmentCount() const {
		return sourceSegments_.size();
	}

	unsigned int destinationSegmentCount() const {
		return destinationSegments_.size();
	}

	virtual void sourceSegmentIs(const Ptr<Segment>& segment) {
		auto res = addSegment(sourceSegments_, segment);
		if (res) {
			post(this, &Notifiee::onSourceSegmentNew, segment);
		}
	}

	virtual void destinationSegmentIs(const Ptr<Segment>& segment) {
		auto res = addSegment(destinationSegments_, segment);
		if (res) {
			post(this, &Notifiee::onDestinationSegmentNew, segment);
		}
	}

	Ptr<Segment> sourceSegmentDel(const Ptr<Segment>& segment) {
		auto seg = deleteSegment( sourceSegments_, segment);
		if (seg != null) {
			post(this, &Notifiee::onSourceSegmentDel, seg);
		}

		return seg;
	}

	iterator sourceSegmentDel(SegmentVector::const_iterator iter) {
		auto seg = *iter;
		auto next = sourceSegments_.erase(iter);

		post(this, &Notifiee::onSourceSegmentDel, seg);

		return next;
	}

	Ptr<Segment> destinationSegmentDel(const Ptr<Segment>& segment) {
		auto seg = deleteSegment( destinationSegments_, segment);
		if (seg != null) {
			post(this, &Notifiee::onDestinationSegmentDel, seg);
		}

		return seg;
	}

	iterator destinationSegmentDel(SegmentVector::const_iterator iter) {
		auto seg = *iter;
		auto next = destinationSegments_.erase(iter);

		post(this, &Notifiee::onDestinationSegmentDel, seg);
		
		return next;
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
		sourceSegments_(0),
		destinationSegments_(0)
	{
		// Nothing to do
	}

	~Location() {
		//TODO: Do we need to free the Segment pointers in segments_. I don't think so.
	}

private:

	Ptr<Segment> findSegment(const SegmentVector& segments, const U32 id) const {
		if (id < segments.size()) {
			return segments[id];
		}

		return null;
	}

	bool addSegment(SegmentVector& segments, const Ptr<Segment>& segment) {
		if (find (segments.begin(), segments.end(), segment) == segments.end()) {
			segments.push_back(segment); 
			return true;
		}

		return false;
	}

	Ptr<Segment> deleteSegment(SegmentVector& segments, const Ptr<Segment>& segment) {
		auto it = find( segments.begin(), segments.end(), segment);
		if (it == segments.end()) {
			return null;
		}

		auto seg = *it;
		segments.erase(it);

		return seg;
	}
	
	/* Segments for which the Location object is the 'source' */
	SegmentVector sourceSegments_;

	/* Segments for which the Location object is the 'destination' */
	SegmentVector destinationSegments_;
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

	virtual void sourceSegmentIs(const Ptr<Segment>& segment) {
		if (dynamic_cast<Road*>(segment.ptr()) != null) {
			Location::sourceSegmentIs(segment);
			return;
		}

		logError(WARNING, "A Residence can be the source of only Road segments. Given segment ('" + segment->name() + "') is not a Road.");
	}

	virtual void destinationSegmentIs(const Ptr<Segment>& segment) {
		if (dynamic_cast<Road*>(segment.ptr()) != null) {
			Location::destinationSegmentIs(segment);
			return;
		}

		logError(WARNING, "A Residence can be the destination of only Road segments. Given segment ('" + segment->name() + "') is not a Road.");
	}

protected:

	explicit Residence(const string& name):
		Location(name)
	{
		// Nothing to do
	}
};

#endif