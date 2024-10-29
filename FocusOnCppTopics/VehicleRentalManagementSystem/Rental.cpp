#include "Rental.hpp"
#include <iostream>

Rental::Rental(std::string id, std::shared_ptr<Vehicle> v, std::shared_ptr<Customer> c, int days)
    : rentalID(id), vehicle(v), customer(c), rentalDays(days) {}

void Rental::displayRentalDetails() const {
    std::cout << "Rental ID: " << rentalID << '\n';
    customer->displayDetails();
    vehicle->displayDetails();
    std::cout << "Rental Days: " << rentalDays << "\nTotal Cost: INR" << (vehicle->getRentalRate() * rentalDays) << '\n';
}

void Rental::saveToFile(std::ofstream& file) const {
    file << rentalID << " " << customer->getCustomerId() << " " << vehicle->getVehicleId() << " " << rentalDays << " " << vehicle->getRentalRate() << '\n';
}