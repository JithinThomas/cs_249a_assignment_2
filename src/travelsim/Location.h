
#ifndef LOCATION_H
#define LOCATION_H

#include <algorithm>
#include <vector>

#include "Segment.h"
#include "ValueTypes.h"

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ptr;

using std::find;
using std::vector;


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
	typedef std::list<Notifiee*> NotifieeList;

public:

	static Ptr<Location> instanceNew(const string& name) {
		return new Location(name);
	}

	Ptr<Segment> sourceSegment(const SegmentId& id) const {
		return findSegment(sourceSegments_, id);
	}

	Ptr<Segment> destinationSegment(const SegmentId& id) const {
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

	NotifieeList& notifiees() {
        return notifiees_;
    }

	Location(const Location&) = delete;

	void operator =(const Location&) = delete;
	void operator ==(const Location&) = delete;

protected:

	virtual void sourceSegmentIs(const Ptr<Segment>& segment) {
		auto res = addSegment(sourceSegments_, segment);
		if (res) {
			if (segment->source() != this) {
				// To avoid infinite chain of method calls due to cicular dependency between segment and location updates
				segment->sourceIs(this);
			}

			post(this, &Notifiee::onSourceSegmentNew, segment);
		}
	}

	virtual void destinationSegmentIs(const Ptr<Segment>& segment) {
		auto res = addSegment(destinationSegments_, segment);
		if (res) {
			if (segment->destination() != this) { 
				// To avoid infinite chain of method calls due to cicular dependency between segment and location updates
				segment->destinationIs(this);
			}

			post(this, &Notifiee::onDestinationSegmentNew, segment);
		}
	}

	Ptr<Segment> sourceSegmentDel(const Ptr<Segment>& segment) {
		auto seg = deleteSegment( sourceSegments_, segment);
		if (seg != null) {
			seg->sourceDel();
			post(this, &Notifiee::onSourceSegmentDel, seg);
		}

		return seg;
	}

	iterator sourceSegmentDel(SegmentVector::const_iterator iter) {
		auto seg = *iter;
		seg->sourceDel();

		auto it = std::find(sourceSegments_.begin(), sourceSegments_.end(), seg);
		auto next = sourceSegments_.erase(it);

		post(this, &Notifiee::onSourceSegmentDel, seg);

		return next;
	}

	void sourceSegmentDelAll() {
		for (auto seg : sourceSegments_) {
			seg->sourceDel();
		}
	}

	Ptr<Segment> destinationSegmentDel(const Ptr<Segment>& segment) {
		auto seg = deleteSegment( destinationSegments_, segment);
		if (seg != null) {
			seg->destinationDel();
			post(this, &Notifiee::onDestinationSegmentDel, seg);
		}

		return seg;
	}

	iterator destinationSegmentDel(SegmentVector::const_iterator iter) {
		auto seg = *iter;
		seg->destinationDel();

		auto it = std::find(destinationSegments_.begin(), destinationSegments_.end(), seg);
		auto next = destinationSegments_.erase(it);

		post(this, &Notifiee::onDestinationSegmentDel, seg);
		
		return next;
	}

	void destinationSegmentDelAll() {
		for (auto seg : destinationSegments_) {
			seg->destinationDel();
		}
	}

	friend class Segment;
	friend class TravelNetworkManager;

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

	Ptr<Segment> findSegment(const SegmentVector& segments, const SegmentId& id) const {
		if (id < segments.size()) {
			return segments[id.value()];
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
