#include "Inventory.hpp"
#include <iostream>

int main() {
    Inventory inventory;

    while(true) {
        std::cout << "Inventory Management System\n";
        std::cout << "1. Add Product\n";
        std::cout << "2. Remove Product\n";
        std::cout << "3. Display Inventory\n";
        std::cout << "4. Exit\n";

        int choice;
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if(choice == 1) {
            // Add Prodcut
            int productID;
            std::string name;
            double price;
            int quantity;
            int type;

            std::cout << "Enter Product ID: ";
            std::cin >> productID;

            std::cout << "Enter Product name: ";
            std::cin.ignore();
            std::getline(std::cin, name);

            std::cout << "Enter Product Price: ";
            std::cin >> price;

            std::cout << "Enter Product Quantity: ";
            std::cin >> quantity;

            std::cout << "Enter Product Type (1 = Perishable, 2 = Non-Perishable): ";
            std::cin >> type;

            if(type == 1) {
                std::string expirationDate;
                std::cout << "Enter Expiration Date (YYYY-MM-DD): ";
                std::cin >> expirationDate;

                inventory.addProduct(new PerishableProduct(productID, name, price, quantity, expirationDate));
            }
            else if(type == 2) {
                inventory.addProduct(new NonPerishableProduct(productID, name, price, quantity));
            }
        }
        else if(choice == 2) {
            // Remove Product
            int productID;
            std::cout << "Enter Product ID to remove: ";
            std::cin >> productID;

            if(inventory.removeProdcut(productID)) {
                std::cout << "Product removed Successfully!\n";
            }
            else {
                std::cout << "Product not found!\n";
            }
        }
        else if(choice == 3) {
            // Display inventory
            inventory.displayInventory();
        }
        else if(choice == 4) {
            std::cout << "Exiting....\n";
            break;
        }
        else {
            std::cout << "Invalid option! Try again.\n";
        }
    }

    return 0;
}