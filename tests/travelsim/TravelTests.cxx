#include "gtest/gtest.h"
#include "fwk/fwk.h"
#include "TravelManager.h"
#include "TravelManagerTracker.h"
#include "Conn.h"

using std::to_string;

TEST(TravelManager, instanceNew) {
	const auto manager = TravelManager::instanceNew("manager-1");
	ASSERT_TRUE(manager != null);
	ASSERT_TRUE(manager->name() == "manager-1");
}

TEST(TravelManager, locationNew) {
	const auto manager = TravelManager::instanceNew("manager-1");
	const auto residenceName = "location-1";
	auto res = manager->location(residenceName);

	ASSERT_TRUE(res == null);

	const auto residence = manager->residenceNew(residenceName);
	res = manager->location(residenceName);

	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == residence);
	ASSERT_TRUE(res->name() == residenceName);
}

TEST(Stats, onResidenceNew) {
	const auto manager = TravelManager::instanceNew("manager-1");
	const auto stats = TravelManagerTracker::instanceNew("stats-1");
	stats->notifierIs(manager);

	ASSERT_EQ(stats->residenceCount(), 0);

	manager->residenceNew("location-1");
	manager->residenceNew("location-2");

	ASSERT_EQ(stats->residenceCount(), 2);
}

TEST(LocationAndSegment, segmentSourceAndDestChanges) {
	const auto manager = TravelManager::instanceNew("manager-1");
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
	const auto manager = TravelManager::instanceNew("manager-1");
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
	const auto manager = TravelManager::instanceNew("manager-1");
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







