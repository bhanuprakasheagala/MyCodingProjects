#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <iostream>
#include <string>

// Base Product Class
class Product {

    protected:
        int productID;
        std::string name;
        double price;
        int quantity;

    public:
        // Constructor
        Product(int id, const std::string& name, double price, int quantity);

        // Virtual destructor
        virtual ~Product() = default;

        // Pure Virtual Method for displaying product details
        virtual void display() const = 0;

        // Getter Methods
        int getProductID() const;
        std::string getName() const;
        double getPrice() const;
        int getQuantity() const;

        // Update product quality
        void updateQuantity(int qty);
};

// Perishable Product Class
class PerishableProduct : public Product {
    private:
        std::string expirationDate;

    public:
        // Constructor
        PerishableProduct(int id, const std::string& name, double price, int quantity, const std::string& expirationDate);

        // Overriding display method
        void display() const override;
};

// Non-Perishable Product Class
class NonPerishableProduct : public Product {
    public:
        // Constructor
        NonPerishableProduct(int id, const std::string& name, double price, int quantity);

        // Overriding display method
        void display() const override;
};

#endif