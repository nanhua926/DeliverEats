/**
 * tests.cpp
 * Author: Yiquan Sun
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CourierBase.h"
#include "GlobalSettings.h"

TEST_CASE("MotorizedCourier Logic Tests", "[moped]") {
    MotorizedCourier moped("FastJim", 30);

    SECTION("Basic attributes") {
        REQUIRE(moped.getName() == "FastJim");
        REQUIRE(moped.getType() == "moped");
    }

    SECTION("Acceptance of priority and distance") {
        // Mopeds should accept everything if distance is reasonable
        REQUIRE(moped.canAccept(1.0, true) == true);
        REQUIRE(moped.canAccept(10.0, false) == true);
    }

    SECTION("Trip duration calculation") {
        // Dist 15 miles -> Round trip 30 miles. 
        // Speed 30mph -> 1 hour -> 60 mins
        REQUIRE(moped.getTripDuration(15.0) == 60);
    }
}

TEST_CASE("PedalCourier Logic Tests", "[bicycle]") {
    // Name, Speed 10mph, Max daily distance 10.0
    PedalCourier bike("SlowSue", 10, 10.0);

    SECTION("Priority restriction") {
        // Bicycles MUST NOT take priority orders
        REQUIRE(bike.canAccept(0.5, true) == false);
    }

    SECTION("One-way distance limit") {
        // Bicycles cannot go over 2 miles one way
        REQUIRE(bike.canAccept(2.0, false) == true);
        REQUIRE(bike.canAccept(2.1, false) == false);
    }

    SECTION("Daily odometer exhaustion") {
        // First trip: 2 miles one way = 4 miles round trip.
        // Used: 4, Max: 10. Remaining: 6.
        bike.updateMileage(2.0);
        
        // Second trip: 2.0 miles = 4 miles round trip. Total used: 8.
        REQUIRE(bike.canAccept(2.0, false) == true);
        bike.updateMileage(2.0);

        // Third trip: 1.5 miles = 3 miles round trip. 
        // 8 + 3 = 11 > 10. Should be rejected.
        REQUIRE(bike.canAccept(1.5, false) == false);
    }
}