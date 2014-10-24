#include "gtest/gtest.h"
#include "fwk/fwk.h"
#include "TravelManager.h"
#include "Stats.h"

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
	const auto stats = Stats::instanceNew("stats-1", manager);

	ASSERT_TRUE(stats->tracker().residenceCount() == 0);

	manager->residenceNew("location-1");
	manager->residenceNew("location-2");

	ASSERT_TRUE(stats->tracker().residenceCount() == 2);
}