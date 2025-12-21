/*
riders.cpp
Author: Yiquan Sun
*/

#include "riders.h"
#include "constants.h"
#include <limits>
#include <cmath>

// === Base Class Implementation ===

Delivery_Rider::Delivery_Rider(std::string name, std::string vehicle_type,
                               int speed, double max_dist)
    : name(name), vehicle_type(vehicle_type), speed_mph(speed),
      max_daily_dist(max_dist), current_daily_dist(0.0)
{
}

bool Delivery_Rider::can_take_order(double distance, bool is_priority) const
{
    // 1. Vehicle Capabilities
    if (vehicle_type == TYPE_BICYCLE) {
        if (is_priority) {
            return false; // Bikes cannot take priority
        }
        if (distance > MAX_ONE_WAY_BIKE_MILES) {
            return false; // Bikes cannot go over 2 miles one way
        }
    }

    // 2. Maximum Distance Check
    double trip_dist = distance * ROUND_TRIP_MULTIPLIER;
    if ((current_daily_dist + trip_dist) > max_daily_dist) {
        return false; // Exceeds daily limit
    }

    return true;
}

void Delivery_Rider::perform_delivery(double one_way_distance)
{
    current_daily_dist += (one_way_distance * ROUND_TRIP_MULTIPLIER);
}

void Delivery_Rider::reset_day()
{
    current_daily_dist = 0.0;
}

int Delivery_Rider::calculate_duration_mins(double distance) const
{
    double total_miles = distance * ROUND_TRIP_MULTIPLIER;
    double hours = total_miles / static_cast<double>(speed_mph);
    // Round to nearest minute
    return static_cast<int>(std::round(hours * MINUTES_IN_HOUR));
}

std::string Delivery_Rider::get_name() const
{
    return name;
}

std::string Delivery_Rider::get_type() const
{
    return vehicle_type;
}

// === Derived Class Implementations ===

Moped_Rider::Moped_Rider(std::string name, int speed)
    : Delivery_Rider(name, TYPE_MOPED, speed, std::numeric_limits<double>::max())
{
}

Bicycle_Rider::Bicycle_Rider(std::string name, int speed, double max_dist)
    : Delivery_Rider(name, TYPE_BICYCLE, speed, max_dist)
{
}
