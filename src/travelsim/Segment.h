
#ifndef SEGMENT_H
#define SEGMENT_H

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ordinal;
using fwk::Ptr;


class Segment;
class Location;

typedef Ordinal< Segment, unsigned int > SegmentLength;

class Segment : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<Segment> {
	public:
		void notifierIs(const Ptr<Segment>& segment) {
			connect(segment, this);
		}

		/* Notification that the 'source' of this segment has been modified */
		virtual void onSource(const Ptr<Location>& prevSource) { }

		/* Notification that the 'destination' of this segment has been modified */
		virtual void onDestination(const Ptr<Location>& prevDestination) { }

		/* Notification that the 'length' of this segment has been modified */
		virtual void onLength() { }
	};

	static Ptr<Segment> instanceNew(const string& name) {
		return new Segment(name);
	}

protected:
	typedef std::list<Notifiee*> NotifieeList;

public:

	Ptr<Location> source() const {
		return source_;
	}

	Ptr<Location> destination() const {
		return destination_;
	}

	SegmentLength length() const {
		return length_;
	}

	void sourceIs(const Ptr<Location>& source) {
		if (source_ != source) {
			const auto prevSource = source_;
			source_ = source;
			post(this, &Notifiee::onSource, prevSource);
		}
	}

	void destinationIs(const Ptr<Location>& destination) {
		if (destination_ != destination) {
			const auto prevDestination = destination_;
			destination_ = destination;
			post(this, &Notifiee::onDestination, prevDestination);
		}
	}

	void lengthIs(const SegmentLength& length) {
		if (length_ != length) {
			length_ = length;
			post(this, &Notifiee::onLength);
		}
	}

	NotifieeList& notifiees() {
        return notifiees_;
    }

	Segment(const Segment&) = delete;

	void operator =(const Segment&) = delete;
	void operator ==(const Segment&) = delete;

protected:

	NotifieeList notifiees_;

	explicit Segment(const string& name) :
		NamedInterface(name),
		length_(0)
	{
		source_ = null;
		destination_ = null;
	}

	~Segment() {}

private:

	Ptr<Location> source_;
	Ptr<Location> destination_;
	SegmentLength length_;
};

class Flight : public Segment {
public:

	static Ptr<Flight> instanceNew(const string& name) {
		return new Flight(name);
	}

protected:

	Flight(const string& name) :
		Segment(name)
	{
		// Nothing to do
	}

	~Flight() { }
};

class Road : public Segment {
public:

	static Ptr<Road> instanceNew(const string& name) {
		return new Road(name);
	}

protected:

	Road(const string& name) :
		Segment(name)
	{
		// Nothing to do
	}

	~Road() {}
};

#endif