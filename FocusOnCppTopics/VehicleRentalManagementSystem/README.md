# Vehicle Rental Management System

This project is a **Vehicle Rental Management System** implemented in C++ using advanced language features such as **object-oriented programming (OOP)**, **polymorphism**, **smart pointers**, **templates**, and **file handling** for data persistence. The system supports adding vehicles, managing customer records, creating rentals, saving/loading data to and from files, and simulating concurrent user interactions.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Class Structure](#class-structure)
  - [VehicleManagementSystem](#vehiclemanagementsystem)
  - [Vehicle (Base Class)](#vehicle-base-class)
  - [Car, Bike, and Truck (Derived Classes)](#car-bike-and-truck-derived-classes)
  - [Customer](#customer)
  - [Rental](#rental)
- [Data Flow](#data-flow)
- [Sorting and File I/O](#sorting-and-file-io)
- [Concurrency](#concurrency)
- [Build and Run](#build-and-run)

---

## Project Overview

The Vehicle Rental Management System manages various vehicle rentals, linking customers to vehicles through rental contracts. It organizes **vehicles** (cars, bikes, trucks), **customers**, and **rentals** using an efficient structure. By leveraging OOP principles, the system maintains a clean, extensible code base, allowing new vehicle types or customer details to be added seamlessly.

## Features

- **Add Vehicles and Customers**: Supports different vehicle types (Cars, Bikes, Trucks) and stores customer data.
- **Create Rentals**: Links customers to vehicles with rental contracts.
- **Sort Vehicles by Rental Rate**: Sorts vehicles by rate using C++ lambda functions.
- **Data Persistence**: Saves and loads vehicle, customer, and rental data to/from text files.
- **Concurrency**: Simulates multiple users creating rentals simultaneously using C++ threads.

## Class Structure

### VehicleManagementSystem

The `VehicleManagementSystem` class orchestrates the entire system, handling the collections of **vehicles**, **customers**, and **rentals**. This class provides core functions to add vehicles, create rentals, display information, save/load data, and simulate user interactions.

```cpp
class VehicleManagementSystem {
    std::map<std::string, std::shared_ptr<Vehicle>> vehicles;
    std::map<std::string, std::shared_ptr<Customer>> customers;
    std::list<std::shared_ptr<Rental>> rentals;

    void createRental(...); // Creates a new rental contract
    void addVehicle<T>(...); // Template method to add vehicles
    void sortVehiclesByRate();
    void saveToFile();
    void loadFromFile();
    void simulateMultipleUsers();
};
```

**Diagram**: Vehicle Management System and its Core Components

```
+--------------------------------------------+
|          VehicleManagementSystem           |
|--------------------------------------------|
| - vehicles: map<string, shared_ptr<Vehicle>> |
| - customers: map<string, shared_ptr<Customer>> |
| - rentals: list<shared_ptr<Rental>>         |
| + createRental()                            |
| + addVehicle<T>()                           |
| + displayAllVehicles()                      |
| + displayAllRentals()                       |
| + sortVehiclesByRate()                      |
| + saveToFile()                              |
| + loadFromFile()                            |
| + simulateMultipleUsers()                   |
+--------------------------------------------+
```

### Vehicle (Base Class)

The `Vehicle` class serves as an abstract base class with derived classes for specific vehicle types. Each derived class (e.g., `Car`, `Bike`, `Truck`) inherits from `Vehicle` and implements its own unique attributes and `displayDetails` functionality.

```cpp
class Vehicle {
    std::string vehicleId;
    std::string model;
    double rentalRatePerDay;
    virtual void displayDetails() const = 0;
};
```

**Diagram**: Inheritance and Composition of Vehicle Classes

```
+-----------+------------------+
|   Vehicle (Base Class)       |
+------------------------------+
| + vehicleId                  |
| + model                      |
| + rentalRatePerDay           |
| + displayDetails() (virtual) |
+------------------------------+
              ^
              |
       +------+------+
       |             |
+------v----+ +------v----+
|    Car    | |    Bike   |
+-----------+ +-----------+
| + numSeats| | + isElectric |
+-----------+ +-----------+
```

### Car, Bike, and Truck (Derived Classes)

These derived classes extend `Vehicle`, adding specific attributes:

- `Car`: Adds `numSeats`.
- `Bike`: Adds `isElectric`.
- `Truck`: Adds `loadCapacity`.

Each class provides its own `displayDetails()` and `saveToFile()` methods.

### Customer

The `Customer` class stores essential customer data, including `customerId`, `name`, and `phoneNumber`. It links to `Rental` to associate each rental with a customer.

```cpp
class Customer {
    std::string customerId;
    std::string name;
    std::string phoneNumber;
    void displayDetails() const;
};
```

**Diagram**: Customer and Rental Association

```
+---------------------+
|      Customer       |
+---------------------+
| - customerId        |
| - name              |
| - phoneNumber       |
+---------------------+
        ^
        |
        | associated
        |
+-----------------------+
|        Rental         |
+-----------------------+
| - rentalId            |
| - rentalDays          |
| - vehicle             |
| - customer            |
+-----------------------+
```

### Rental

The `Rental` class represents rental agreements, storing `rentalId`, `rentalDays`, and shared pointers to associated `Vehicle` and `Customer` objects.

---

## Data Flow

### Initialization and Loading Data

```cpp
VehicleManagementSystem system;
system.loadFromFile();
```

1. Loads data from `vehicles.txt`, `customers.txt`, and `rentals.txt`.
2. Each line is parsed, and `Vehicle`, `Customer`, or `Rental` objects are instantiated and stored in collections.

### Rental Creation Flow

```cpp
system.createRental("RENT001", "CUST001", "CAR101", 3);
```

1. Validates `customerId` and `vehicleId`.
2. Creates a `Rental` instance and adds it to `rentals`.
3. Updates the availability of the vehicle if needed.

**Diagram**: Rental Creation Flow

```
+--------------------------------+
|  VehicleManagementSystem       |
| + createRental(...)            |
+--------------------------------+
        |
        | Retrieves Customer, Vehicle
        v
+------------------------+
|       Rental           |
+------------------------+
| Linked to Customer, Vehicle |
+------------------------+
```

## Sorting and File I/O

### Sorting Vehicles by Rate

Sorting is performed using a lambda function in `sortVehiclesByRate`, which organizes vehicles in descending order of `rentalRatePerDay`.

```cpp
system.sortVehiclesByRate();
```

### Saving to File

Data is persisted using `saveToFile()`:

- `vehicles.txt`: Saves all `Vehicle` data.
- `customers.txt`: Saves all `Customer` data.
- `rentals.txt`: Saves all `Rental` data.

**Diagram**: Save to File Workflow

```
+-------------------------------+
|  VehicleManagementSystem      |
| + saveToFile()                |
+-------------------------------+
        |
        |
        +---> vehicles.txt
        +---> customers.txt
        +---> rentals.txt
```

## Concurrency

The `simulateMultipleUsers()` method simulates concurrent users creating rentals using **C++ threads**. This demonstrates multithreading by allowing different users to interact with the system simultaneously.

---

## Build and Run

### Requirements

- C++11 or later
- Makefile support

### Build and Execute

1. Clone the repository.
   ```sh
   git clone <repository_url>
   cd VehicleRentalSystem
   ```

2. Compile the program.
   ```sh
   make
   ```

3. Run the program.
   ```sh
   ./VehicleRentalSystem
   ```

## Project Components and Relationships

+--------------------------------------------+
|          VehicleManagementSystem           |
|--------------------------------------------|
| - vehicles: map<string, shared_ptr<Vehicle>> |
| - customers: map<string, shared_ptr<Customer>> |
| - rentals: list<shared_ptr<Rental>>         |
| + createRental()                            |
| + addVehicle<T>()                           |
| + addCustomer()                             |
| + displayAllVehicles()                      |
| + displayAllRentals()                       |
| + sortVehiclesByRate()                      |
| + saveToFile()                              |
| + loadFromFile()                            |
| + simulateMultipleUsers()                   |
+--------------------------------------------+
            ^                ^           ^
            |                |           |
           has              has          |
            |                |           |
+-----------+----+      +----+-----------+      
|                |      |                |
|                |      |                |
|   +------------v------+-----------+    |
|   |       Vehicle                 |    |
|   +-------------------------------+    |
|   | - vehicleId: string           |    |
|   | - model: string               |    |
|   | - rentalRatePerDay: double    |    |
|   | + displayDetails()            |    |
|   | + getVehicleId()              |    |
|   | + getRentalRate()             |    |
|   +-------------------------------+    |
|              ^                          |
|              | inherits                 |
| +------------+------+-----------+       |
| |                    |           |      |
| |                    |           |      |
| |                    |           |      |
| |                    |           |      |
| v                    v           v      v
+-------------------+ +----------------+ +-----------------+
|     Car           | |      Bike      | |     Truck      |
|-------------------| |----------------| |-----------------|
| - numSeats: int   | | - isElectric:  | | - loadCapacity:|
|                   | |   bool         | |   double       |
+-------------------+ +----------------+ +-----------------+

+---------------------+
|      Customer       |
|---------------------|
| - customerId: string|
| - name: string      |
| - phoneNumber: string|
| + displayDetails()  |
| + getCustomerId()   |
+---------------------+
                ^
                |
                | associated
                |
+----------------------------------+
|             Rental               |
|----------------------------------|
| - rentalId: string               |
| - rentalDays: int                |
| - vehicle: shared_ptr<Vehicle>   |
| - customer: shared_ptr<Customer> |
| + displayRentalDetails()         |
| + saveToFile()                   |
+----------------------------------+
