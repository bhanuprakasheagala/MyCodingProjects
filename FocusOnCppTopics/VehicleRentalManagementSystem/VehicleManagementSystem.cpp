#include "VehicleManagementSystem.hpp"
#include "InvalidIdException.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>

VehicleManagementSystem::VehicleManagementSystem() {}

void VehicleManagementSystem::addCustomer(Customer* customer) {
    std::lock_guard<std::mutex> lock(mtx);
    customers[customer->getCustomerId()] = std::shared_ptr<Customer>(customer);
}

void VehicleManagementSystem::createRental(std::string rentalID, std::string customerId, std::string vehicleId, int days) {
    std::lock_guard<std::mutex> lock(mtx);
    auto custIt = customers.find(customerId);
    auto vehIt = vehicles.find(vehicleId);

    if(custIt == customers.end()) {
        throw InvalidIDException("Customer ID " + customerId + " not found");
    }
    if(vehIt == vehicles.end()) {
        throw InvalidIDException("Vehicle ID " + vehicleId + " not found");
    }

    auto rental = std::make_shared<Rental>(rentalID, vehIt->second, custIt->second, days);
    rentals.push_back(rental);

}

void VehicleManagementSystem::displayAllVehicles() const {
    for(const auto& pair: vehicles) {
        pair.second->displayDetails();
        std::cout << "------------------------------\n";
    }
}

void VehicleManagementSystem::displayAllRentals() const {
    for(const auto& rental : rentals) {
        rental->displayRentalDetails();
        std::cout << "------------------------------\n";
    }
}

void VehicleManagementSystem::saveToFile() const {
    std::ofstream vehicleFile("vehicle.txt");
    for(const auto& pair : vehicles) {
        pair.second->saveToFile(vehicleFile);
    }
    vehicleFile.close();

    std::ofstream customerFile("customer.txt");
    for(const auto& pair : customers) {
        pair.second->saveToFile(customerFile);
    }
    customerFile.close();

    std::ofstream rentalFile("rentals.txt");
    for(const auto& rental : rentals) {
        rental->saveToFile(rentalFile);
    }
    rentalFile.close();
}

void VehicleManagementSystem::loadFromFile() {
    std::ifstream vehicleFile("vehicle.txt");
    std::string type, id, model;
    double rate;

    while(vehicleFile >> type >> id >> model >> rate) {
        if(type == "Car") {
            int seats;
            vehicleFile >> seats;
            addVehicle(new Car(id, model, rate, seats));
        }
        else if(type == "Bike") {
            bool electric;
            vehicleFile >> electric;
            addVehicle(new Bike(id, model, rate, electric));
        }
        else if(type == "Truck") {
            double capacity;
            vehicleFile >> capacity;
            addVehicle(new Truck(id, model, rate, capacity));
        }
    }
    vehicleFile.close();

    std::ifstream customerFile("customers.txt");
    std::string custId, name, phone;
    while(customerFile >> custId >> name >> phone) {
        addCustomer(new Customer(custId, name, phone));
    }
    customerFile.close();

    std::ifstream rentalFile("rentals.txt");
    std::string rentalId, custIdRent, vehId;
    int days;
    while(rentalFile >> rentalId >> custIdRent >> vehId >> days) {
        createRental(rentalId, custIdRent, vehId, days);
    }
    rentalFile.close();
}

// Sorting vehicles by rate using lambda function
void VehicleManagementSystem::sortVehicleByRate() const {
    std::vector<std::shared_ptr<Vehicle>> vehicleList;
    for(const auto& pair : vehicles) {
        vehicleList.push_back(pair.second);
    }

    std::sort(vehicleList.begin(), vehicleList.end(), [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b){
        return a->getRentalRate() < b->getRentalRate();
    });

    for(const auto& vehicle : vehicleList) {
        vehicle->displayDetails();
        std::cout << "----------------------------------\n";
    }
}

// Simulating multiple user interactiong using Concurrency
void VehicleManagementSystem::simulateMultipleUsers() {
    std::thread t1([&]() {
        try {
            createRental("Rent03", "Cust01", "Car101", 2);
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    });

    std::thread t2([&] () {
        try {
            createRental("RENT004", "CUST002", "BIKE201", 1);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    });

    t1.join();
    t2.join();
}