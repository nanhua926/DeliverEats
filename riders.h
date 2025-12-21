/*
riders.h
Author:Yiquan Sun
*/

#ifndef _RIDERS_H_
#define _RIDERS_H_

#include <string>

/**
 * @brief Base class representing a generic delivery rider.
 */
class Delivery_Rider
{
protected:
    std::string name;             ///< Name of the rider
    std::string vehicle_type;     ///< Type of vehicle (Moped/Bicycle)
    int speed_mph;                ///< Speed in miles per hour
    double max_daily_dist;        ///< Max miles allowed per day
    double current_daily_dist;    ///< Miles travelled so far today

public:
    /**
     * @brief Constructor for Delivery_Rider.
     * @param name Name of the rider.
     * @param vehicle_type Type of vehicle.
     * @param speed Speed in mph.
     * @param max_dist Maximum daily mileage (infinity for mopeds).
     */
    Delivery_Rider(std::string name, std::string vehicle_type, 
                   int speed, double max_dist);

    virtual ~Delivery_Rider() {}

    /**
     * @brief Checks if the rider can accept a specific order.
     * @param distance The one-way distance of the order.
     * @param is_priority True if the order requires priority handling.
     * @return True if the rider can take the order, false otherwise.
     */
    bool can_take_order(double distance, bool is_priority) const;

    /**
     * @brief Updates the rider's daily mileage.
     * @param one_way_distance The distance of the order (one way).
     */
    void perform_delivery(double one_way_distance);

    /**
     * @brief Resets daily mileage to zero.
     */
    void reset_day();

    /**
     * @brief Calculates how long the rider will be gone.
     * @param distance The one-way distance.
     * @return Time in minutes (for round trip).
     */
    int calculate_duration_mins(double distance) const;

    // Getters
    std::string get_name() const;
    std::string get_type() const;
};

/**
 * @brief Derived class for Moped riders.
 */
class Moped_Rider : public Delivery_Rider
{
public:
    /**
     * @brief Constructor for Moped_Rider.
     * @param name Name of the rider.
     * @param speed Speed in mph.
     */
    Moped_Rider(std::string name, int speed);
};

/**
 * @brief Derived class for Bicycle riders.
 */
class Bicycle_Rider : public Delivery_Rider
{
public:
    /**
     * @brief Constructor for Bicycle_Rider.
     * @param name Name of the rider.
     * @param speed Speed in mph.
     * @param max_dist Max mileage before stopping.
     */
    Bicycle_Rider(std::string name, int speed, double max_dist);
};

#endif
