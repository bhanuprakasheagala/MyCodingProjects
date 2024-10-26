#include <iostream>
#include "Vehicle.hpp"

Vehicle::Vehicle(std::string id, std::string mdl, double rate) : vehicleId(id), model(mdl), rentalRatePerDay(rate) {}

std::string Vehicle::getVehicleId() const {
    return vehicleId;
}

double Vehicle::getRentalRate() const {
    return rentalRatePerDay;
}

bool Vehicle::operator>(const Vehicle& other) const {
    return rentalRatePerDay > other.rentalRatePerDay;
}

// Car class implementation
Car::Car(std::string id, std::string mdl, double rate, int seats) : Vehicle(id, mdl, rate), numSeats(seats) {}

void Car::displayDetails() const {
    std::cout << "Car ID: " << vehicleId << "\nModel: " << model << "\nSeats: " << numSeats << "\nRate per day: INR" << rentalRatePerDay << '\n';
}

void Car::saveToFile(std::ofstream& file) const {
    file << "Car " << vehicleId << " " << model << " " << rentalRatePerDay << " " << numSeats << '\n';
}

// Bike class implementation
Bike::Bike(std::string id, std::string mdl, double rate, bool electric) : Vehicle(id, mdl, rate), isElectric(electric) {}

void Bike::displayDetails() const {
    std::cout << "Bike ID: " << vehicleId << "\nModel: " << model << "\nIs Electric: " << (isElectric ? "Yes" : "No") << "\nRate per day: INR"<< rentalRatePerDay << '\n';
}

void Bike::saveToFile(std::ofstream& file) const {
    file << "Bike " << vehicleId << " " << model << " " << rentalRatePerDay << " " << isElectric << '\n';
}

// Truck class implementation
Truck::Truck(std::string id, std::string mdl, double rate, double capacity) : Vehicle(id, mdl, rate), loadCapacity(capacity) {}

void Truck::displayDetails() const {
    std::cout << "Truck ID: " << vehicleId << "\nModel: " << model << "\n Load Capacity: " << loadCapacity << "\n Rate per day: INR" << rentalRatePerDay << '\n';
}

void Truck::saveToFile(std::ofstream& file) const {
    file << "Truck: " << vehicleId << " " << model << " " << rentalRatePerDay << " " << loadCapacity << '\n';
}

