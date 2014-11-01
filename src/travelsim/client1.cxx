#include "TravelInstanceManager.h"

#include <ostream>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

Ptr<Instance> segmentNew(
    const Ptr<InstanceManager>& manager, const string& name, const string& spec, 
    const string& source, const string& dest, const double length
) {
    const auto seg = manager->instanceNew(name, spec);
    seg->attributeIs("source", source);
    seg->attributeIs("destination", dest);
    seg->attributeIs("length", std::to_string(length));
    return seg;
}

int main(const int argc, const char* const argv[]) {
    const auto manager = TravelInstanceManager::instanceManager();

    const auto stats = manager->instanceNew("myStats", "Stats");

    const auto plane = manager->instanceNew("plane", "Airplane");
    plane->attributeIs("speed", "500");
    plane->attributeIs("capacity", "200");
    plane->attributeIs("cost", "40");

    const auto car = manager->instanceNew("car", "Car");
    car->attributeIs("speed", "70");
    car->attributeIs("capacity", "5");
    car->attributeIs("cost", "0.75");


    manager->instanceNew("stanford", "Residence");  
    manager->instanceNew("menlopark", "Residence");  

    manager->instanceNew("sfo", "Airport");  
    manager->instanceNew("lax", "Airport");  

    segmentNew(manager, "carSeg1", "Road", "stanford", "sfo", 20);
    segmentNew(manager, "carSeg2", "Road", "sfo", "stanford", 20);
    segmentNew(manager, "carSeg3", "Road", "menlopark", "stanford", 20);
    segmentNew(manager, "carSeg4", "Road", "sfo", "menlopark", 20);
    segmentNew(manager, "carSeg5", "Road", "stanford", "menlopark", 5);
    segmentNew(manager, "carSeg6", "Road", "menlopark", "stanford", 5);
    segmentNew(manager, "flightSeg1", "Flight", "sfo", "lax", 350);

    manager->instance("carSeg2")->attributeIs("length", "40");

    cout << "carSeg1.source: ";
    cout << manager->instance("carSeg1")->attribute("source") << endl;

    cout << "segment1 of sfo: ";
    cout << manager->instance("sfo")->attribute("segment1") << endl; 

    cout << "Stats:" << endl;
    cout << "# Residences: " << stats->attribute("Residence") << endl;
    cout << "# Airports: " << stats->attribute("Airport") << endl;
    cout << "# Road segments: " << stats->attribute("Road") << endl;
    cout << "# Flight segments: " << stats->attribute("Flight") << endl;

    const auto conn = manager->instanceNew("myConn", "Conn");

    const auto query1 = "explore sfo distance 500";
    const auto query2 = "explore sfo distance 20";
    const auto query3 = "explore sfo distance 10";
    const auto query4 = "explore lax distance 1000";

    cout << "**** " << query1 << " ****" << endl;
    cout << conn->attribute(query1) << endl;

    cout << "**** " << query2 << " ****" << endl;
    cout << conn->attribute(query2) << endl;

    cout << "**** " << query3 << " ****" << endl;
    cout << conn->attribute(query3) << endl;

    cout << "**** " << query4 << " ****" << endl;
    cout << conn->attribute(query4) << endl;

    // =====================================================
    //  Instructions with errorneous inputs
    // =====================================================

    // Segment attributes
    manager->instance("carSeg2")->attributeIs("length", "40ert");
    manager->instance("carSeg2")->attributeIs("length", "a40sd");
    manager->instance("carSeg2")->attributeIs("length", "jkl40");
    manager->instance("carSeg2")->attributeIs("source", "jfk");
    manager->instance("carSeg2")->attributeIs("destination", "wret");
    manager->instance("carSeg2")->attributeIs("length", "-23");
    manager->instance("carSeg2")->attributeIs("length", "23");
    manager->instance("carSeg2")->attributeIs("length", "-17.234");
    manager->instance("carSeg2")->attributeIs("length", "17.234");
    manager->instance("carSeg1")->attribute("qwerty");

    // Vehicle attributes
    manager->instance("car")->attributeIs("speed", "-12.802");
    manager->instance("car")->attributeIs("capacity", "-49");
    manager->instance("car")->attributeIs("cost", "-57.19");
    manager->instance("car")->attribute("qwerty");

    // Location attributes
    manager->instance("stanford")->attribute("uiuuo");
    manager->instance("sfo")->attribute("wqer");

    // Conn attributes
    conn->attribute("bnml");

    // Stats attributes
    stats->attribute("plsdf");

    return 0;
}
