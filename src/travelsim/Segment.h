
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

	static Ptr<Segment> instanceNew(const string& name, 
									const Ptr<Location>& source, 
									const Ptr<Location>& destination,
									const SegmentLength& length) {
		return new Segment(name, source, destination, length);
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

protected:

	NotifieeList notifiees_;

	Segment(const string& name, 
			const Ptr<Location>& source, 
			const Ptr<Location>& destination,
			const SegmentLength& length) :
		NamedInterface(name),
		source_(source),
		destination_(destination),
		length_(length)
	{
		// Nothing to do
	}

	~Segment() {}

private:
	Ptr<Location> source_;
	Ptr<Location> destination_;
	SegmentLength length_;
};

class Flight : public Segment {
public:
	static Ptr<Flight> instanceNew(	const string& name, 
									const Ptr<Location>& source, 
									const Ptr<Location>& destination,
									const SegmentLength& length) {
		return new Flight(name, source, destination, length);
	}

protected:

	Flight(const string& name, 
			const Ptr<Location>& source, 
			const Ptr<Location>& destination,
			const SegmentLength& length) :
		Segment(name, source, destination, length)
	{
		// Nothing to do
	}
};

class Road : public Segment {
public:
	static Ptr<Road> instanceNew(	const string& name, 
									const Ptr<Location>& source, 
									const Ptr<Location>& destination,
									const SegmentLength& length) {
		return new Road(name, source, destination, length);
	}

protected:
	Road(const string& name, 
			const Ptr<Location>& source, 
			const Ptr<Location>& destination,
			const SegmentLength& length) :
		Segment(name, source, destination, length)
	{
		// Nothing to do
	}

	~Road() {}
};

#endif