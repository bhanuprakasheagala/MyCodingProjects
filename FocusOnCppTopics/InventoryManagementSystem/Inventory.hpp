#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Product.hpp"
#include <vector>

class Inventory {
    private:
        std::vector<Product*> products;

    public:
        // Destructor to delete dynamically allocated products
        ~Inventory();

        // Add product to the inventory
        void addProduct(Product* product);

        // Remove product by ID
        bool removeProduct(int productID);

        // Display all products in the inventory
        void displayInventory() const;

        // Find product by ID
        Product* findProduct(int productID) const;
};

#endif