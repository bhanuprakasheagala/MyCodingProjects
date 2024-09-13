#include "Product.hpp"

// Base Product Constructor
Product::Product(int id, const std::string& name, double price, int quantity)
    : productID(id), name(name), price(price), quantity(quantity) {}

// Getter Methods
int Product::getProductID() const {
    return productID;
}
std::string Product::getName() const {
    return name;
}
double Product::getPrice() const {
    return price;
}
int Product::getQuantity() const {
    return quantity;
}

// Update quantity
void Product::updateQuantity(int qty) {
    quantity = qty;
}

// PerishableProduct Constructor
PerishableProduct::PerishableProduct(int id, const std::string& name, double price, int quantity, const std::string& expirationDate)
    : Product(id, name, price, quantity), expirationDate(expirationDate) {}

// Display method for Perishable Product
void PerishableProduct::display() const {
    std::cout << "Perishable Product: " << name << "\nID: " << productID << "\nPrice: INR " << price
    << "\nQuantity: " << quantity << "\nExpiration Date: " << expirationDate << '\n';
}

// NonPerishableProduct Constructor
NonPerishableProduct::NonPerishableProduct(int id, const std::string& name, double price, int quantity)
    : Product(id, name, price, quantity) {}

// Display method for NonPerishableProduct
void NonPerishableProduct::display() const {
    std::cout << "Non-Perishable Product: " << name << "\nID: " << productID << "\nPrice: INR " << price
    << "\nQuantity: " << quantity << '\n';
}