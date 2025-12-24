#ifndef _COURIER_BASE_H_
#define _COURIER_BASE_H_

#include <string>

/**
 * CourierBase.h
 * Author: Yiquan Sun
 * Created: 24/12/2025
 */

class BaseCourier {
private:
    std::string courierName;
    std::string vehicleType;
    int speedMph;
    double maxDailyMiles;
    double milesDrivenToday;

public:
    BaseCourier(std::string name, std::string type, int speed, double limit);
    virtual ~BaseCourier() {}

    /**
     * Checks if the courier is capable of taking the specific order.
     * @param distance The one-way distance.
     * @param isPriority If the order is priority.
     * @return true if capable.
     */
    virtual bool canAccept(double distance, bool isPriority) const;

    /**
     * Updates the courier's mileage after a successful delivery.
     * @param distance One-way distance.
     */
    void updateMileage(double distance);

    /**
     * Resets the courier's daily data for a new day.
     */
    void refresh();

    /**
     * Calculates trip duration in minutes.
     * @param dist One-way distance.
     * @return duration in minutes.
     */
    int getTripDuration(double dist) const;

    // Getters
    std::string getName() const { return courierName; }
    std::string getType() const { return vehicleType; }
};

class MotorizedCourier : public BaseCourier {
public:
    MotorizedCourier(std::string name, int speed);
};

class PedalCourier : public BaseCourier {
public:
    PedalCourier(std::string name, int speed, double limit);
    bool canAccept(double distance, bool isPriority) const override;
};

#endif