#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include <fstream>
#include <string>

class Customer {
private:
    std::string customerId;
    std::string name;
    std::string phoneNumber;
public:
    Customer(std::string id, std::string nm, std::string phone);
    void displayDetails() const;
    std::string getCustomerId() const;
    void saveToFile(std::ofstream& file) const;
};

#endif // CUSTOMER_HPP
