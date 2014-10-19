#include "gtest/gtest.h"
#include "fwk/fwk.h"
#include "TravelManager.h"

TEST(TravelManager, instanceNew) {
	const auto manager = TravelManager::instanceNew("manager-1");
	ASSERT_TRUE(manager != null);
	ASSERT_TRUE(manager->name() == "manager-1");
}

TEST(TravelManager, locationNew) {
	const auto manager = TravelManager::instanceNew("manager-1");
	const auto locationName = "location-1";
	auto res = manager->location(locationName);

	ASSERT_TRUE(res == null);

	const auto location = manager->locationNew(locationName);
	res = manager->location(locationName);

	ASSERT_TRUE(res != null);
	ASSERT_TRUE(res == location);
	ASSERT_TRUE(res->name() == locationName);
}