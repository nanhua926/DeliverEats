#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <memory>
#include "CourierBase.h"
#include "GlobalSettings.h"
#include <algorithm>
/**
 * DispatchSystem.cpp
 * Author: Yiquan Sun
 */

struct ActiveMission {
    int returnTime;
    BaseCourier* courier;
};

struct DailyBalance {
    int totalDelivered = 0;
    double totalMoney = 0.0;
    int bikeCount = 0;
    double bikeMoney = 0.0;
    int mopedCount = 0;
    double mopedMoney = 0.0;
    int missedOrders = 0;

    void reset() { *this = DailyBalance(); }
};

/**
 * Writes the daily statistics to a .dat file.
 * @param date The date string (e.g., 12/01/2026).
 * @param data The statistics object.
 */
void generateReport(const std::string& date, const DailyBalance& data) {
    if (date.empty()) return;
    std::string fileName = "";
    for (char c : date) if (isdigit(c)) fileName += c;
    fileName += ".dat";

    std::ofstream ofs(fileName);
    if (ofs.is_open()) {
        ofs << "Total deliveries: " << data.totalDelivered << "\n"
            << "Total money: " << data.totalMoney << "\n"
            << "Bicycle deliveries: " << data.bikeCount << "\n"
            << "Bicycle money: " << data.bikeMoney << "\n"
            << "Moped deliveries: " << data.mopedCount << "\n"
            << "Moped money: " << data.mopedMoney << "\n"
            << "Missed orders: " << data.missedOrders << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Initialize master courier list according to the table in PDF
    std::vector<std::unique_ptr<BaseCourier>> roster;
    roster.push_back(std::make_unique<MotorizedCourier>("Anne", 32));
    roster.push_back(std::make_unique<MotorizedCourier>("Jim", 28));
    roster.push_back(std::make_unique<PedalCourier>("Sue", 4, 23));
    roster.push_back(std::make_unique<PedalCourier>("Bill", 5, 17));
    roster.push_back(std::make_unique<MotorizedCourier>("James", 25));
    roster.push_back(std::make_unique<MotorizedCourier>("Amy", 24));
    roster.push_back(std::make_unique<MotorizedCourier>("Bob", 27));
    roster.push_back(std::make_unique<PedalCourier>("Steve", 3, 21));

    std::deque<BaseCourier*> queue;
    std::vector<ActiveMission> activeMissions;
    DailyBalance stats;
    std::string currentDay = "";

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: File could not be opened." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '\r') continue;

        if (line.find('/') != std::string::npos) {
            // New Day Transition
            generateReport(currentDay, stats);
            currentDay = line;
            stats.reset();
            activeMissions.clear();
            queue.clear();
            for (auto& c : roster) {
                c->refresh();
                queue.push_back(c.get());
            }
        } else {
            // Process Order: HH.MM:Dist:Type
            std::replace(line.begin(), line.end(), ':', ' ');
            std::replace(line.begin(), line.end(), '.', ' ');
            std::stringstream ss(line);
            int hr, min; double dist; std::string type;
            ss >> hr >> min >> dist >> type;

            int currentTime = hr * 60 + min;
            bool priority = (type == AppConfig::TYPE_PRIORITY);

            // 1. Process Returnees
            auto it = activeMissions.begin();
            while (it != activeMissions.end()) {
                if (it->returnTime <= currentTime) {
                    queue.push_back(it->courier);
                    it = activeMissions.erase(it);
                } else {
                    ++it;
                }
            }

            // 2. Dispatch Logic
            BaseCourier* assigned = nullptr;
            for (auto q_it = queue.begin(); q_it != queue.end(); ++q_it) {
                if ((*q_it)->canAccept(dist, priority)) {
                    assigned = *q_it;
                    queue.erase(q_it);
                    break;
                }
            }

            // 3. Update Accounting
            if (assigned) {
                double fee = priority ? AppConfig::FEE_PRIORITY : AppConfig::FEE_STANDARD;
                assigned->updateMileage(dist);
                activeMissions.push_back({currentTime + assigned->getTripDuration(dist), assigned});

                stats.totalDelivered++;
                stats.totalMoney += fee;
                if (assigned->getType() == "bicycle") {
                    stats.bikeCount++; stats.bikeMoney += fee;
                } else {
                    stats.mopedCount++; stats.mopedMoney += fee;
                }
            } else {
                stats.missedOrders++;
            }
        }
    }
    generateReport(currentDay, stats);
    return 0;
}