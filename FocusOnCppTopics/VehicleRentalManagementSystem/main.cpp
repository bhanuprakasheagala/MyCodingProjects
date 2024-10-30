#include "VehicleManagementSystem.hpp"
#include "Vehicle.hpp"
#include "Customer.hpp"
#include "Rental.hpp"

int main() {

    VehicleManagementSystem vms;
    vms.loadFromFile();

    // Sample vehicles and customers for testing
    vms.addVehicle(new Car("CAR101", "Tesla Model S", 100, 4));
    vms.addVehicle(new Bike("Bike201", "HD Ride", 50, false));
    vms.addVehicle(new Truck("Truck301", "Ford F-150", 150, 5));

    vms.addCustomer(new Customer("Cust1", "Alice", "555-145"));
    vms.addCustomer(new Customer("Cust2", "Bob", "678-132"));
    vms.addCustomer(new Customer("Cust3", "Fred", "536-845"));

    try {
        vms.createRental("Rent001", "Cust1", "Bike201", 3);
        vms.createRental("Rent002", "Cust3", "Truck301", 4);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    vms.displayAllVehicles();
    vms.displayAllRentals();

    vms.sortVehicleByRate();
    vms.saveToFile();
    vms.simulateMultipleUsers();

    return 0;
}