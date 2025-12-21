/*
tests.cpp
Unit tests for Deliver Eats classes
Author: Yiquan Sun
Created: 24/10/2025
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "riders.h"
#include "constants.h"

TEST_CASE("Moped Rider Functionality", "[moped]") {
    Moped_Rider m("MopedTest", 30); // 30 mph

    SECTION("Initialization") {
        REQUIRE(m.get_name() == "MopedTest");
        REQUIRE(m.get_type() == "Moped");
    }

    SECTION("Can take priority") {
        REQUIRE(m.can_take_order(1.0, true) == true);
    }

    SECTION("Can take long distance") {
        REQUIRE(m.can_take_order(100.0, false) == true);
    }

    SECTION("Time Calculation") {
        // Dist 15 miles -> Round trip 30 miles
        // Speed 30mph -> 1 hour -> 60 mins
        REQUIRE(m.calculate_duration_mins(15.0) == 60);
    }
}

TEST_CASE("Bicycle Rider Functionality", "[bicycle]") {
    // Name, Speed 10mph, Max Dist 10 miles
    Bicycle_Rider b("BikeTest", 10, 10.0);

    SECTION("Cannot take priority") {
        REQUIRE(b.can_take_order(1.0, true) == false);
    }

    SECTION("One way distance limit") {
        REQUIRE(b.can_take_order(2.0, false) == true);
        REQUIRE(b.can_take_order(2.1, false) == false);
    }

    SECTION("Max daily distance logic") {
        // 1. Take order: 2 miles (4 miles round trip)
        // Used: 4, Remaining: 6
        REQUIRE(b.can_take_order(2.0, false) == true);
        b.perform_delivery(2.0);

        // 2. Take order: 2 miles (4 miles round trip)
        // Used: 8, Remaining: 2
        REQUIRE(b.can_take_order(2.0, false) == true);
        b.perform_delivery(2.0);

        // 3. Attempt order: 1.5 miles (3 miles round trip)
        // Used: 8 + 3 = 11 > 10. Should fail.
        REQUIRE(b.can_take_order(1.5, false) == false);
    }
}