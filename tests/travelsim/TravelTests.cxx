#include "gtest/gtest.h"
#include "fwk/fwk.h"
#include "TravelNetworkManager.h"
#include "Conn.h"

using std::to_string;

TEST(TravelNetworkManager, instanceNew) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	ASSERT_TRUE(manager != null);
	ASSERT_TRUE(manager->name() == "manager-1");
}

TEST(TravelNetworkManager, locationNew) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto residence1 = "location-1";
	const auto airport1 = "location-2";

	ASSERT_TRUE(manager->location(residence1) == null);
	ASSERT_TRUE(manager->location(airport1) == null);

	const auto residence = manager->residenceNew(residence1);
	const auto airport = manager->airportNew(airport1);
	auto res = manager->location(residence1);

	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == residence);
	ASSERT_TRUE(res->name() == residence1);

	res = manager->location(airport1);
	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == airport);
	ASSERT_TRUE(res->name() == airport1);

	/* Testing airport() and residence() accessor methods */
	auto res1 = manager->airport(airport1);
	ASSERT_TRUE(res1 == airport);
	ASSERT_TRUE(res1->name() == airport1);

	auto res2 = manager->residence(residence1);
	ASSERT_TRUE(res2 == residence);
	ASSERT_TRUE(res2->name() == residence1);
}

TEST(TravelNetworkManager, segmentNew) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto flight1 = "segment-1";
	const auto road1 = "segment-2";

	ASSERT_TRUE(manager->segment(flight1) == null);
	ASSERT_TRUE(manager->segment(road1) == null);

	const auto flight = manager->flightNew(flight1);
	const auto road = manager->roadNew(road1);
	auto res = manager->segment(road1);

	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == road);
	ASSERT_TRUE(res->name() == road1);

	res = manager->segment(flight1);
	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == flight);
	ASSERT_TRUE(res->name() == flight1);

	/* Testing flight() and road() accessor methods */
	auto res1 = manager->flight(flight1);
	ASSERT_TRUE(res1 == flight);
	ASSERT_TRUE(res1->name() == flight1);

	auto res2 = manager->road(road1);
	ASSERT_TRUE(res2 == road);
	ASSERT_TRUE(res2->name() == road1);
}

TEST(TravelNetworkManager, vehicleNew) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto car1 = "vehicle-1";
	const auto airplane1 = "vehicle-2";

	ASSERT_TRUE(manager->segment(car1) == null);
	ASSERT_TRUE(manager->segment(airplane1) == null);

	const auto car = manager->carNew(car1);
	const auto airplane = manager->airplaneNew(airplane1);
	auto res = manager->vehicle(car1);

	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == car);
	ASSERT_TRUE(res->name() == car1);

	res = manager->vehicle(airplane1);
	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == airplane);
	ASSERT_TRUE(res->name() == airplane1);

	/* Testing car() and airplane() accessor methods */
	auto res1 = manager->car(car1);
	ASSERT_TRUE(res1 == car);
	ASSERT_TRUE(res1->name() == car1);

	auto res2 = manager->airplane(airplane1);
	ASSERT_TRUE(res2 == airplane);
	ASSERT_TRUE(res2->name() == airplane1);
}

TEST(TravelNetworkManager, locationIter) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "location1", "location2", "location3", "location4" };

	manager->airportNew(names[0]);
	manager->residenceNew(names[1]);
	manager->residenceNew(names[2]);
	manager->airportNew(names[3]);

	ASSERT_EQ(names.size(), 4);

	for (auto it = manager->locationIter(); it != manager->locationIterEnd(); it++) {
		const auto loc = it->second;
		auto tmp = std::find(names.begin(), names.end(), loc->name());
		names.erase(tmp);
	}

	ASSERT_EQ(names.size(), 0);
}

TEST(TravelNetworkManager, segmentIter) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "segment1", "segment2", "segment3", "segment4" };

	manager->flightNew(names[0]);
	manager->roadNew(names[1]);
	manager->flightNew(names[2]);
	manager->roadNew(names[3]);

	ASSERT_EQ(names.size(), 4);

	for (auto it = manager->segmentIter(); it != manager->segmentIterEnd(); it++) {
		const auto seg = it->second;
		auto tmp = std::find(names.begin(), names.end(), seg->name());
		names.erase(tmp);
	}

	ASSERT_EQ(names.size(), 0);
}

TEST(TravelNetworkManager, vehicleIter) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "segment1", "segment2", "segment3", "segment4" };

	manager->carNew(names[0]);
	manager->airplaneNew(names[1]);
	manager->airplaneNew(names[2]);
	manager->carNew(names[3]);

	ASSERT_EQ(names.size(), 4);

	for (auto it = manager->vehicleIter(); it != manager->vehicleIterEnd(); it++) {
		const auto v = it->second;
		auto tmp = std::find(names.begin(), names.end(), v->name());
		names.erase(tmp);
	}

	ASSERT_EQ(names.size(), 0);
}

TEST(TravelNetworkManager, locationDel) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "location1", "location2", "location3", "location4" };

	const auto loc0 = manager->airportNew(names[0]);
	const auto loc1 = manager->residenceNew(names[1]);
	const auto loc2 = manager->residenceNew(names[2]);
	const auto loc3 = manager->airportNew(names[3]);

	auto res = manager->locationDel(names[2]);

	ASSERT_TRUE(res == loc2);
	ASSERT_TRUE(manager->location(names[2]) == null);
	ASSERT_TRUE(loc2 != null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->location(names[0]) == loc0);
	ASSERT_TRUE(manager->location(names[1]) == loc1);
	ASSERT_TRUE(manager->location(names[3]) == loc3);

	res = manager->locationDel(names[0]);

	ASSERT_TRUE(res == loc0);
	ASSERT_TRUE(manager->location(names[0]) == null);
	ASSERT_TRUE(loc0 != null);

	res = manager->locationDel("fakeName");
	ASSERT_TRUE(res == null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->location(names[1]) == loc1);
	ASSERT_TRUE(manager->location(names[3]) == loc3);
}

TEST(TravelNetworkManager, segmentDel) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "segment1", "segment2", "segment3", "segment4" };

	const auto seg0 = manager->roadNew(names[0]);
	const auto seg1 = manager->flightNew(names[1]);
	const auto seg2 = manager->flightNew(names[2]);
	const auto seg3 = manager->roadNew(names[3]);

	auto res = manager->segmentDel(names[2]);

	ASSERT_TRUE(res == seg2);
	ASSERT_TRUE(manager->segment(names[2]) == null);
	ASSERT_TRUE(seg2 != null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->segment(names[0]) == seg0);
	ASSERT_TRUE(manager->segment(names[1]) == seg1);
	ASSERT_TRUE(manager->segment(names[3]) == seg3);

	res = manager->segmentDel(names[0]);

	ASSERT_TRUE(res == seg0);
	ASSERT_TRUE(manager->segment(names[0]) == null);
	ASSERT_TRUE(seg0 != null);

	res = manager->segmentDel("fakeName");
	ASSERT_TRUE(res == null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->segment(names[1]) == seg1);
	ASSERT_TRUE(manager->segment(names[3]) == seg3);
}

TEST(TravelNetworkManager, vehicleDel) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> names { "vehicle1", "vehicle2", "vehicle3", "vehicle4" };

	const auto v0 = manager->carNew(names[0]);
	const auto v1 = manager->airplaneNew(names[1]);
	const auto v2 = manager->carNew(names[2]);
	const auto v3 = manager->airplaneNew(names[3]);

	auto res = manager->vehicleDel(names[1]);

	ASSERT_TRUE(res == v1);
	ASSERT_TRUE(manager->vehicle(names[1]) == null);
	ASSERT_TRUE(v1 != null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->vehicle(names[0]) == v0);
	ASSERT_TRUE(manager->vehicle(names[2]) == v2);
	ASSERT_TRUE(manager->vehicle(names[3]) == v3);

	res = manager->vehicleDel(names[0]);

	ASSERT_TRUE(res == v0);
	ASSERT_TRUE(manager->vehicle(names[0]) == null);
	ASSERT_TRUE(v0 != null);

	res = manager->vehicleDel("fakeName");
	ASSERT_TRUE(res == null);

	/* Ensure that the other locations remain intact */
	ASSERT_TRUE(manager->vehicle(names[2]) == v2);
	ASSERT_TRUE(manager->vehicle(names[3]) == v3);
}

TEST(TravelNetworkManager, locationDel_Update_Segment_SrcAndDst) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> locationNames { "location0", "location1", "location2" };
	vector<string> segmentNames { "segment0", "segment1", "segment2" };

	const auto loc0 = manager->airportNew(locationNames[0]);
	const auto loc1 = manager->residenceNew(locationNames[1]);
	const auto loc2 = manager->airportNew(locationNames[2]);

	const auto seg0 = manager->flightNew(segmentNames[0]);
	const auto seg1 = manager->roadNew(segmentNames[1]);
	const auto seg2 = manager->roadNew(segmentNames[2]);

	seg0->sourceIs(loc0);
	seg0->destinationIs(loc2);

	seg1->sourceIs(loc0);
	seg1->destinationIs(loc1);

	seg2->sourceIs(loc1);
	seg2->destinationIs(loc2);

	ASSERT_TRUE(seg0->source() == loc0);
	ASSERT_TRUE(seg0->destination() == loc2);

	ASSERT_TRUE(seg1->source() == loc0);
	ASSERT_TRUE(seg1->destination() == loc1);

	ASSERT_TRUE(seg2->source() == loc1);
	ASSERT_TRUE(seg2->destination() == loc2);

	manager->locationDel(locationNames[0]);
	manager->locationDel(locationNames[2]);

	ASSERT_TRUE(seg0->source() == null);
	ASSERT_TRUE(seg0->destination() == null);

	ASSERT_TRUE(seg1->source() == null);
	ASSERT_TRUE(seg1->destination() == loc1);

	ASSERT_TRUE(seg2->source() == loc1);
	ASSERT_TRUE(seg2->destination() == null);
}

TEST(TravelNetworkManager, segmentDel_Update_Loc_SrcAndDst) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> locationNames { "location0", "location1", "location2" };
	vector<string> segmentNames { "segment0", "segment1", "segment2" };

	const auto loc0 = manager->airportNew(locationNames[0]);
	const auto loc1 = manager->residenceNew(locationNames[1]);
	const auto loc2 = manager->airportNew(locationNames[2]);

	const auto seg0 = manager->flightNew(segmentNames[0]);
	const auto seg1 = manager->roadNew(segmentNames[1]);
	const auto seg2 = manager->roadNew(segmentNames[2]);

	seg0->sourceIs(loc0);
	seg0->destinationIs(loc2);

	seg1->sourceIs(loc0);
	seg1->destinationIs(loc1);

	seg2->sourceIs(loc1);
	seg2->destinationIs(loc2);

	ASSERT_TRUE(loc0->sourceSegment(0) == seg0);
	ASSERT_TRUE(loc2->destinationSegment(0) == seg0);

	manager->segmentDel(segmentNames[0]);

	ASSERT_EQ(loc0->sourceSegmentCount(), 1);
	ASSERT_EQ(loc0->destinationSegmentCount(), 0);

	ASSERT_EQ(loc1->sourceSegmentCount(), 1);
	ASSERT_EQ(loc1->destinationSegmentCount(), 1);

	ASSERT_EQ(loc2->sourceSegmentCount(), 0);
	ASSERT_EQ(loc2->destinationSegmentCount(), 1);

	ASSERT_TRUE(loc0->sourceSegment(0) == seg1);
	ASSERT_TRUE(loc0->sourceSegment(1) == null);
	ASSERT_TRUE(loc0->destinationSegment(0) == null);

	ASSERT_TRUE(loc1->sourceSegment(0) == seg2);
	ASSERT_TRUE(loc1->sourceSegment(1) == null);
	ASSERT_TRUE(loc1->destinationSegment(0) == seg1);
	ASSERT_TRUE(loc1->destinationSegment(1) == null);

	ASSERT_TRUE(loc2->sourceSegment(0) == null);
	ASSERT_TRUE(loc2->destinationSegment(0) == seg2);
	ASSERT_TRUE(loc2->destinationSegment(1) == null);
}


TEST(Stats, onLocation) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto stats = manager->stats();
	vector<string> names { "location1", "location2", "location3", "location4", "location5" };

	ASSERT_EQ(stats->locationCount(), 0);
	ASSERT_EQ(stats->airportCount(), 0);
	ASSERT_EQ(stats->residenceCount(), 0);

	manager->airportNew(names[0]);
	manager->residenceNew(names[1]);
	manager->residenceNew(names[2]);
	manager->airportNew(names[3]);
	manager->residenceNew(names[4]);

	ASSERT_EQ(stats->locationCount(), 5);
	ASSERT_EQ(stats->airportCount(), 2);
	ASSERT_EQ(stats->residenceCount(), 3);

	manager->locationDel(names[2]);

	ASSERT_EQ(stats->locationCount(), 4);
	ASSERT_EQ(stats->airportCount(), 2);
	ASSERT_EQ(stats->residenceCount(), 2);

	manager->locationDel(names[0]);
	manager->locationDel(names[3]);

	ASSERT_EQ(stats->locationCount(), 2);
	ASSERT_EQ(stats->airportCount(), 0);
	ASSERT_EQ(stats->residenceCount(), 2);

	manager->locationDel(names[0]);
	manager->locationDel(names[3]);

	ASSERT_EQ(stats->locationCount(), 2);
	ASSERT_EQ(stats->airportCount(), 0);
	ASSERT_EQ(stats->residenceCount(), 2);
}

TEST(Stats, onSegment) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto stats = manager->stats();
	vector<string> names { "segment1", "segment2", "segment3", "segment4", "segment5" };

	ASSERT_EQ(stats->segmentCount(), 0);
	ASSERT_EQ(stats->flightCount(), 0);
	ASSERT_EQ(stats->roadCount(), 0);

	manager->roadNew(names[0]);
	manager->flightNew(names[1]);
	manager->roadNew(names[2]);
	manager->flightNew(names[3]);
	manager->roadNew(names[4]);

	ASSERT_EQ(stats->segmentCount(), 5);
	ASSERT_EQ(stats->flightCount(), 2);
	ASSERT_EQ(stats->roadCount(), 3);

	manager->segmentDel(names[2]);
	manager->segmentDel(names[1]);
	manager->segmentDel(names[4]);

	ASSERT_EQ(stats->segmentCount(), 2);
	ASSERT_EQ(stats->flightCount(), 1);
	ASSERT_EQ(stats->roadCount(), 1);

	manager->segmentDel(names[1]);
	manager->segmentDel(names[4]);

	ASSERT_EQ(stats->segmentCount(), 2);
	ASSERT_EQ(stats->flightCount(), 1);
	ASSERT_EQ(stats->roadCount(), 1);
}

TEST(Stats, onVehicle) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto stats = manager->stats();
	vector<string> names { "vehicle1", "vehicle2", "vehicle3", "vehicle4", "vehicle5" };

	ASSERT_EQ(stats->vehicleCount(), 0);
	ASSERT_EQ(stats->airplaneCount(), 0);
	ASSERT_EQ(stats->carCount(), 0);

	manager->carNew(names[0]);
	manager->airplaneNew(names[1]);
	manager->carNew(names[2]);
	manager->airplaneNew(names[3]);
	manager->carNew(names[4]);

	ASSERT_EQ(stats->vehicleCount(), 5);
	ASSERT_EQ(stats->airplaneCount(), 2);
	ASSERT_EQ(stats->carCount(), 3);

	manager->vehicleDel(names[1]);
	manager->vehicleDel(names[2]);
	manager->vehicleDel(names[4]);

	ASSERT_EQ(stats->vehicleCount(), 2);
	ASSERT_EQ(stats->airplaneCount(), 1);
	ASSERT_EQ(stats->carCount(), 1);

	manager->vehicleDel(names[2]);
	manager->vehicleDel(names[4]);

	ASSERT_EQ(stats->vehicleCount(), 2);
	ASSERT_EQ(stats->airplaneCount(), 1);
	ASSERT_EQ(stats->carCount(), 1);
}

TEST(LocationAndSegment, SegmentCounts) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	vector<string> locationNames { "location1", "location2", "location3" };
	vector<string> segmentNames { "segment1", "segment2", "segment3" };

	const auto loc0 = manager->airportNew(locationNames[0]);
	const auto loc1 = manager->residenceNew(locationNames[1]);
	const auto loc2 = manager->airportNew(locationNames[2]);

	const auto seg0 = manager->flightNew(segmentNames[0]);
	const auto seg1 = manager->roadNew(segmentNames[1]);
	const auto seg2 = manager->roadNew(segmentNames[2]);

	seg0->sourceIs(loc0);
	seg0->destinationIs(loc2);

	seg1->sourceIs(loc0);
	seg1->destinationIs(loc1);

	seg2->sourceIs(loc1);
	seg2->destinationIs(loc2);

	ASSERT_EQ(loc0->sourceSegmentCount(), 2);
	ASSERT_EQ(loc0->destinationSegmentCount(), 0);

	ASSERT_EQ(loc1->sourceSegmentCount(), 1);
	ASSERT_EQ(loc1->destinationSegmentCount(), 1);

	ASSERT_EQ(loc2->sourceSegmentCount(), 0);
	ASSERT_EQ(loc2->destinationSegmentCount(), 2);
}

TEST(LocationAndSegment, segmentSourceAndDestChanges) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto loc1 = manager->residenceNew("residence-1");
	const auto loc2 = manager->residenceNew("residence-2");
	const auto loc3 = manager->residenceNew("residence-3");
	const auto loc4 = manager->airportNew("airport-1");
	const auto loc5 = manager->airportNew("airport-2");

	const auto seg1 = manager->roadNew("segment-1");
	seg1->sourceIs(loc1);
	seg1->destinationIs(loc2);

	ASSERT_EQ(loc1->sourceSegmentCount(), 1);
	ASSERT_EQ(loc1->destinationSegmentCount(), 0);

	ASSERT_EQ(loc2->sourceSegmentCount(), 0);
	ASSERT_EQ(loc2->destinationSegmentCount(), 1);

	ASSERT_EQ(loc1->sourceSegment(0), seg1);
	ASSERT_EQ(loc2->destinationSegment(0), seg1);

	seg1->sourceIs(loc3);
	seg1->destinationIs(loc4);

	ASSERT_EQ(loc1->sourceSegmentCount(), 0);
	ASSERT_EQ(loc1->destinationSegmentCount(), 0);

	ASSERT_EQ(loc2->sourceSegmentCount(), 0);
	ASSERT_EQ(loc2->destinationSegmentCount(), 0);

	ASSERT_EQ(loc3->sourceSegmentCount(), 1);
	ASSERT_EQ(loc3->destinationSegmentCount(), 0);

	ASSERT_EQ(loc4->sourceSegmentCount(), 0);
	ASSERT_EQ(loc4->destinationSegmentCount(), 1);

	ASSERT_EQ(loc3->sourceSegment(0),seg1);
	ASSERT_EQ(loc4->destinationSegment(0), seg1);
}

TEST(LocationAndSegment, multipleSegments) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto loc1 = manager->airportNew("location-1");
	const auto loc2 = manager->airportNew("location-2");
	const auto loc3 = manager->residenceNew("location-3");

	const auto seg1 = manager->roadNew("segment-1");
	seg1->sourceIs(loc1);
	seg1->destinationIs(loc2);

	const auto seg2 = manager->roadNew("segment-2");
	seg2->sourceIs(loc1);
	seg2->destinationIs(loc3);

	const auto seg3 = manager->roadNew("segment-3");
	seg3->sourceIs(loc2);
	seg3->destinationIs(loc3);

	ASSERT_EQ(loc1->sourceSegmentCount(), 2);
	ASSERT_EQ(loc1->destinationSegmentCount(), 0);

	ASSERT_EQ(loc2->sourceSegmentCount(), 1);
	ASSERT_EQ(loc2->destinationSegmentCount(), 1);

	ASSERT_EQ(loc3->sourceSegmentCount(), 0);
	ASSERT_EQ(loc3->destinationSegmentCount(), 2);
}

TEST(LocationAndSegment, locationDel) {
	const auto manager = TravelNetworkManager::instanceNew("manager-1");
	const auto loc1 = manager->residenceNew("location-1");
	const auto loc2 = manager->airportNew("location-2");
	const auto loc3 = manager->residenceNew("location-3");

	const auto seg1 = manager->roadNew("segment-1");
	seg1->sourceIs(loc2);
	seg1->destinationIs(loc3);

	const auto seg2 = manager->roadNew("segment-2");
	seg2->sourceIs(loc1);
	seg2->destinationIs(loc2);

	ASSERT_EQ(seg1->source(), loc2);
	ASSERT_EQ(seg1->destination(), loc3);

	ASSERT_EQ(seg2->source(), loc1);
	ASSERT_EQ(seg2->destination(), loc2);

	manager->locationDel(loc3->name());

	ASSERT_EQ(seg1->source(), loc2);
	ASSERT_EQ(seg1->destination(), null);

	manager->locationDel(loc2->name());	

	ASSERT_EQ(seg1->source(), null);
	ASSERT_EQ(seg1->destination(), null);

	ASSERT_EQ(seg2->source(), loc1);
	ASSERT_EQ(seg2->destination(), null);
}







