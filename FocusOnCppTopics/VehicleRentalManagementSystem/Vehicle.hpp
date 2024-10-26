#ifndef VEHICLE_HPP
#define  VEHICLE_HPP

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

class Vehicle {
protected:
    std::string vehicleId;
    std::string model;
    double rentalRatePerDay;

public:
    Vehicle(std::string id, std::string mdl, double rate);
    virtual void displayDetails() const = 0;
    std::string getVehicleId() const;
    virtual double getRentalRate() const;
    bool operator>(const Vehicle& other) const;
    virtual void saveToFile(std::ofstream& file) const = 0;
    virtual ~Vehicle() = default; 
};

class Car : public Vehicle {
private:
    int numSeats;
public:
    Car(std::string id, std::string mdl, double rate, int seats);
    void displayDetails() const override;
    void saveToFile(std::ofstream& file) const override;   
};

class Bike : public Vehicle {
private:
    bool isElectric;
public:
    Bike(std::string id, std::string mdl, double rate, bool electric);  
    void displayDetails() const override;
    void saveToFile(std::ofstream& file) const override;
};

class Truck : public Vehicle {
    private:
        double loadCapacity;
    public:
        Truck(std::string id, std::string mdl, double rate, double capacity);
        void displayDetails() const override;
        void saveToFile(std::ofstream& file) const override;
};

#endif