#include "Inventory.hpp"

// Destructor to free memory
Inventory::~Inventory() {
    for(auto& product : products) {
        delete product;
    }
}

// Add product to the inventory
void Inventory::addProduct(Prodcut* product) {
    products.push_back(product);
}

// Remove product by ID
bool Inventory::removeProduct(int productID) {
    for(auto it = products.begin(); it != products.end(); ++it) {
        if((*it)->getProductID() == productID) {
            delete *it;
            products.erase(it);
            return true;
        }
    }

    return false;
}

// Display all products
void Inventory::displayInventory() const {
    if(products.empty()) {
        std::cout << "Inventory is empty!\n";
        return;
    }
    for(const auto& product : products) {
        products->display();
        std::cout << "------------------------------\n";
    }
}

// Find product by ID
Prodcut* Inventory::findProduct(int productID) const {
    for(const auto& product : products) {
        if(product->getProductID() == productID) {
            return product;
        }
    }

    return nullptr;
}

