#include "CourierBase.h"
#include "GlobalSettings.h"
#include <cmath>
#include <limits>

/**
 * CourierBase.cpp
 * Author: Yiquan Sun
 */

using namespace AppConfig;

BaseCourier::BaseCourier(std::string name, std::string type, int speed, double limit)
    : courierName(name), vehicleType(type), speedMph(speed), 
      maxDailyMiles(limit), milesDrivenToday(0.0) {}

bool BaseCourier::canAccept(double distance, bool isPriority) const {
    double totalTrip = distance * TRIP_MULTIPLIER;
    return (milesDrivenToday + totalTrip) <= maxDailyMiles;
}

void BaseCourier::updateMileage(double distance) {
    milesDrivenToday += (distance * TRIP_MULTIPLIER);
}

void BaseCourier::refresh() {
    milesDrivenToday = 0.0;
}

int BaseCourier::getTripDuration(double dist) const {
    double totalDist = dist * TRIP_MULTIPLIER;
    double hours = totalDist / static_cast<double>(speedMph);
    return static_cast<int>(std::round(hours * MINUTES_PER_HOUR));
}

MotorizedCourier::MotorizedCourier(std::string name, int speed)
    : BaseCourier(name, "moped", speed, std::numeric_limits<double>::max()) {}

PedalCourier::PedalCourier(std::string name, int speed, double limit)
    : BaseCourier(name, "bicycle", speed, limit) {}

bool PedalCourier::canAccept(double distance, bool isPriority) const {
    if (isPriority || distance > BIKE_MAX_ONE_WAY) {
        return false;
    }
    return BaseCourier::canAccept(distance, isPriority);
}