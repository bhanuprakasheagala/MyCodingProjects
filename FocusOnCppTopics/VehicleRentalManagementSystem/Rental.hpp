#ifndef RENTAL_HPP
#define RENTAL_HPP

#include "Vehicle.hpp"
#include "Customer.hpp"
#include <memory>
#include <fstream>

class Rental {
private:
    std::string rentalID;
    std::shared_ptr<Vehicle> vehicle;
    std::shared_ptr<Customer> customer;
    int rentalDays;

public:
    Rental(std::string id, std::shared_ptr<Vehicle> v, std::shared_ptr<Customer> c, int days);
    void displayRentalDetails() const;
    void saveToFile(std::ofstream& file) const;
};

#endif