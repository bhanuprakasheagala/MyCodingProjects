#ifndef VEHICLE_MANAGEMENT_SYSTEM_HPP
#define VEHICLE_MANAGEMENT_SYSTEM_HPP

#include "Vehicle.hpp"
#include "Customer.hpp"
#include "Rental.hpp"
#include <map>
#include <list>
#include <mutex>
#include <memory>

class VehicleManagementSystem {
private:
    std::map<std::string, std::shared_ptr<Vehicle>> vehicles;
    std::map<std::string, std::shared_ptr<Customer>> customers;
    std::list<std::shared_ptr<Rental>> rentals;
    mutable std::mutex mtx;

public:
    VehicleManagementSystem();

    template<typename T>
    void addVehicle(T* vehicle) {
        std::lock_guard<std::mutex> lock(mtx);
        vehicles[vehicle->getVehicleId()] = std::shared_ptr<Vehicle>(vehicle);
    }

    void addCustomer(Customer* customer);
    void createRental(std::string rentalID, std::string customerID, std::string vehicleID, int days);
    void displayAllRentals() const;
    void displayAllVehicles() const;
    void saveToFile() const;
    void loadFromFile() const;
    void sortVehicleByRate() const;
    void simulateMultipleUsers();
};

//#include "VehicleManagementSystem.tpp"
#endif