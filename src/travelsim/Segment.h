
#ifndef SEGMENT_H
#define SEGMENT_H

using fwk::BaseNotifiee;
using fwk::NamedInterface;
using fwk::NotifierLib::post;
using fwk::Ordinal;
using fwk::Ptr;

class Location;
class Segment;

typedef Ordinal< Segment, unsigned int > SegmentLength;

class Segment : public NamedInterface {
public:

	class Notifiee : public BaseNotifiee<Segment> {
	public:
		void notifierIs(const Ptr<Segment>& segment) {
			connect(segment, this);
		}

		/* Notification that the 'source' of this segment has been modified */
		virtual void onSource() { }

		/* Notification that the 'destination' of this segment has been modified */
		virtual void onDestination() { }

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
		source_ = source;
	}

	void destinationIs(const Ptr<Location>& destination) {
		destination_ = destination;
	}

	void lengthIs(const SegmentLength& length) {
		length_ = length;
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