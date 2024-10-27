#include "Customer.hpp"
#include <iostream>

Customer::Customer(std::string id, std::string nm, std::string phone) : customerId(id), name(nm), phoneNumber(phone) {}

void Customer::displayDetails() const {
    std::cout << "Customer ID: " << customerId << "\nName: " << name << "\nPhone: " << phoneNumber << '\n';
}

std::string Customer::getCustomerId() const {
    return customerId;
}

void Customer::saveToFile(std::ofstream& file) const{
    file << customerId << " " << name << " " << phoneNumber << '\n';
}