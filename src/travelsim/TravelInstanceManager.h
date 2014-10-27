#ifndef TRAVELSIM_TRAVELINSTANCEMANAGER_H
#define TRAVELSIM_TRAVELINSTANCEMANAGER_H

#include "InstanceManager.h"
#include "TravelManager.h"
#include "TravelManagerTracker.h"
#include "Conn.h"

#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::to_string;


const string nameOfEntity(Ptr<NamedInterface> entity) {
    if (entity != null) {
        return entity->name();
    }

    return "";
} 

/**
 * InstanceManager implementation for the travel simulation.
 */
class TravelInstanceManager : public virtual InstanceManager {
public:

    /**
     * Return an instance manager for the travel simulation.
     */
    static Ptr<InstanceManager> instanceManager() {
        if (instanceManager_ == null) {
            instanceManager_ = new TravelInstanceManager();
            instanceManager_->initialize();
        }

        return instanceManager_;
    }

protected:

    typedef std::invalid_argument InvalidArgumentException;

    class LocationInstance : public Instance {
    public:

        _noinline
        string attribute(const string& name) {
            const auto i = segmentNumber(name) - 1;
            const auto segment = location_->segment(i);

            return nameOfEntity(segment);
        }

        _noinline
        void attributeIs(const string& name, const string& value) {
            cerr << "[WARNING]: The segment attribute of a Location is read-only." << endl;
        }

    protected:

        Ptr<Location> location() const {
            return location_;
        }

        void locationIs(const Ptr<Location>& location) {
            location_ = location;
        }

        LocationInstance(const string& name) :
            Instance(name)
        {
            location_ = null;
        }

    private:

        friend class TravelInstanceManager;


        static const string segmentStr;

        static const unsigned long segmentStrlen;

        Ptr<Location> location_;

        // TODO: Finish the implementation.


        _noinline
        int segmentNumber(const string& name) {
            if (name.substr(0, segmentStrlen) == segmentStr) {
                // TODO: Check that name doesn't have extraneous characters.
                return std::stoi(name.c_str() + segmentStrlen);
            }

            return 0;
        }

    };


    class SegmentInstance : public Instance {
    public:

        _noinline
        string attribute(const string& name) {
            if (name == "source") {
                return nameOfEntity(segment_->source());
            } else if (name == "destination") {
                return nameOfEntity(segment_->destination());
            } else if (name == "length") {
                return to_string(segment_->length().value());
            }

            cerr << "[WARNING]: A Segment does not have an attribute named '" << name << "'" << endl;
            return "";
        }

        _noinline
        void attributeIs(const string& name, const string& value) {
            if (name == "source") {
                if (segment_ != null) {
                    segment_->sourceIs(travelManager_->location(value));
                }
            } else if (name == "destination") {
                if (segment_ != null) {
                    segment_->destinationIs(travelManager_->location(value));
                }
            } else if (name == "length") {
                if (segment_ != null) {
                    auto length = SegmentLength(stoi(value));
                    segment_->lengthIs(length);
                }
            }
        }

    protected:

        Ptr<Segment> segment() const {
            return segment_;
        }

        void segmentIs(const Ptr<Segment>& segment) {
            segment_ = segment;
        }

        SegmentInstance(const string& name) :
            Instance(name)
        {
            segment_ = null;
        }

    private:

        friend class TravelInstanceManager;

        Ptr<Segment> segment_;
        // TODO: Finish the implementation.

    };


    class VehicleInstance : public Instance {
    public:

        _noinline
        string attribute(const string& name) {
            if (name == "capacity") {
                // TODO: Finish the implementation.
                return "";
            }

            // TODO: Finish the implementation.

            return "";
        }

        _noinline
        void attributeIs(const string& name, const string& value) {
            if (name == "capacity") {
                // TODO: Finish the implementation.
                return;
            }

            // TODO: Finish the implementation.
        }

    protected:

        Ptr<Vehicle> vehicle() const {
            return vehicle_;
        }

        void vehicleIs(const Ptr<Vehicle>& vehicle) {
            vehicle_ = vehicle;
        }

        VehicleInstance(const string& name) :
            Instance(name)
        {
            vehicle_ = null;
        }

    private:

        friend class TravelInstanceManager;

        Ptr<Vehicle> vehicle_;

    };


    class ConnInstance : public Instance {
    public:

        _noinline
        string attribute(const string& name) {
            using std::stringstream;

            stringstream ss(name);

            string cmd;
            ss >> cmd;
            if (cmd != "explore") {
                return "";
            }

            string locName;
            ss >> locName;

            // TODO: Finish the implementation.

            return "";
        }

        _noinline
        void attributeIs(const string& name, const string& value) {
            cerr << "[WARNING]: Writes are not allowed to Conn entities." << endl;
        }

    protected:

        Ptr<Conn> conn() const {
            return conn_;
        }

        void connIs(const Ptr<Conn>& conn) {
            conn_ = conn;
        }

        explicit ConnInstance(const string& name) :
            Instance(name)
        {
           conn_ = null; 
        }

    private:

        friend class TravelInstanceManager;

        Ptr<Conn> conn_;
        // TODO: Finish the implementation.

    };

    class StatsInstance : public Instance {
    public:

        _noinline
        string attribute(const string& name) {

            return "";
        }

        _noinline
        void attributeIs(const string& name, const string& value) {
            cerr << "[WARNING]: Writes are not allowed to Stats entities." << endl;
        }

    protected:

        Ptr<TravelManagerTracker> stats() const {
            return stats_;
        }

        void statsIs(const Ptr<TravelManagerTracker>& stats) {
            stats_ = stats;
        }

        StatsInstance(const string& name) :
            Instance(name)
        {
           stats_ = null; 
        }

    private:

        friend class TravelInstanceManager;

        Ptr<TravelManagerTracker> stats_;
        // TODO: Finish the implementation.

    };


    static Ptr<TravelInstanceManager> instanceManager_;


    /**
     * Initialize any state in the manager.
     */
    void initialize() {
        // TODO: Finish the implementation.
        travelManager_ = TravelManager::instanceNew("manager");

        statsInstance_ = new StatsInstance("stats");
        auto travelManagerTracker = TravelManagerTracker::instanceNew("stats");
        travelManagerTracker->notifierIs(travelManager_);
        statsInstance_->statsIs(travelManagerTracker);

        connInstance_ = new ConnInstance("conn");
        auto conn = Conn::instanceNew("conn");
        conn->travelManagerIs(travelManager_);
        connInstance_->connIs(conn);
    }

    /**
     * Returns a newly-created instance with the specified name.
     * The spec parameter indicates additional information about
     * the instance, such as its type.
     */
    // TODO: Finish the implementation.
    Ptr<Instance> createInstance(const string& name, const string& spec) {
        if (spec == "Residence") {
            auto residence = Residence::instanceNew(name);
            return createLocationInstance(name, residence);
        } else if (spec == "Airport") {
            auto airport = Airport::instanceNew(name);
            return createLocationInstance(name, airport);
        } else if (spec == "Flight") {
            auto flight = Flight::instanceNew(name);
            return createSegmentInstance(name, flight);
        } else if (spec == "Road") {
            auto road = Road::instanceNew(name);
            return createSegmentInstance(name, road);
        }  else if (spec == "Airplane") {
            auto airplane = Airplane::instanceNew(name);
            return createVehicleInstance(name, airplane);
        }  else if (spec == "Car") {
            auto car = Car::instanceNew(name);
            return createVehicleInstance(name, car);
        }  else if (spec == "Conn") {
            return connInstance_;
        }  else if (spec == "Stats") {
            return statsInstance_;
        } 

        cerr << "[WARNING]: Unexpected spec for createInstance(): '" << spec << "'" << endl;

        return null;
    }

private:

    Ptr<LocationInstance> createLocationInstance(const string& name, const Ptr<Location> location) {
        auto instance = new LocationInstance(name);
        instance->locationIs(location);
        instance->travelManagerIs(travelManager_);
        return instance;
    }

    Ptr<SegmentInstance> createSegmentInstance(const string& name, const Ptr<Segment> segment) {
        auto instance = new SegmentInstance(name);
        instance->segmentIs(segment);
        instance->travelManagerIs(travelManager_);
        return instance;
    }

    Ptr<VehicleInstance> createVehicleInstance(const string& name, const Ptr<Vehicle> vehicle) {
        auto instance = new VehicleInstance(name);
        instance->vehicleIs(vehicle);
        instance->travelManagerIs(travelManager_);
        return instance;
    }

    Ptr<TravelManager> travelManager_;
    Ptr<StatsInstance> statsInstance_;
    Ptr<ConnInstance> connInstance_;

};

const string TravelInstanceManager::LocationInstance::segmentStr = "segment";
const unsigned long TravelInstanceManager::LocationInstance::segmentStrlen =
    segmentStr.length();

Ptr<TravelInstanceManager> TravelInstanceManager::instanceManager_;

#endif
