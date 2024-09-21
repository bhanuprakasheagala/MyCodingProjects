# Inventory Management System

## Project Overview

This is a C++ project that implements a simple Inventory Management System for a store. The project utilizes **Object-Oriented Programming (OOP)** principles, including **encapsulation**, **inheritance**, **polymorphism**, and **abstraction**. The system allows users to manage both perishable and non-perishable products, using classes to represent these different product types.

## Key Features

- **Product Management**: Add, remove, and display products.
- **Inventory Management**: Maintain a collection of products in the inventory.
- **Product Types**:
  - **Perishable Products**: Products with an expiration date.
  - **Non-Perishable Products**: Products that do not expire.

## Code Structure

### Classes:

- **Product**: The base class that defines common product attributes (`ID`, `name`, `price`, and `quantity`).
- **PerishableProduct**: A derived class from `Product`, adding an `expirationDate` attribute.
- **NonPerishableProduct**: A derived class from `Product` without any additional attributes.
- **Inventory**: Manages a collection of `Product` pointers and provides functions to add, remove, and display products.

## File Structure:

```bash
├── Product.hpp      # Header file for Product class and derived classes
├── Product.cpp      # Implementation file for Product class and derived classes
├── Inventory.hpp    # Header file for Inventory class
├── Inventory.cpp    # Implementation file for Inventory class
└── main.cpp         # Main program file
```

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/inventory-management-system.git
   ```
2. Compile the code using g++:
   ```bash
   g++ main.cpp Product.cpp Inventory.cpp -o inventory_system
   ```
3. Run the executable:
   ```bash
   ./inventory_system
   ```

---

## Program Flow

Upon running the program, users are greeted with a menu that allows them to:
1. Add a product to the inventory.
2. Remove a product by its ID.
3. Display the entire inventory.
4. Exit the program.

The user is prompted for details such as product type, name, price, quantity, and expiration date (for perishable products).

---

## Diagrams

### 1. UML Class Diagram

This diagram represents the relationship between the classes in the project.

```
+-----------------------+
|      Product           |
+-----------------------+
| - productID            |
| - name                 |
| - price                |
| - quantity             |
+-----------------------+
| + display() : void     |
+-----------------------+
        / \
        | |
+---------------------------+         +---------------------------+
|    PerishableProduct       |         |   NonPerishableProduct     |
+---------------------------+         +---------------------------+
| - expirationDate           |         |                           |
+---------------------------+         +---------------------------+
| + display() : void         |         | + display() : void         |
+---------------------------+         +---------------------------+

+---------------------------+
|        Inventory           |
+---------------------------+
| - products : vector<Product*> |
+---------------------------+
| + addProduct() : void      |
| + removeProduct() : bool   |
| + displayInventory() : void|
+---------------------------+
```

### 2. Program Flowchart

This flowchart describes the main flow of the program, where users can add, remove, or display products.

```
+---------------------------+
| Start                     |
+---------------------------+
        |
        v
+---------------------------+
| Display Menu Options       |
+---------------------------+
        |
        v
+---------------------------+
| User Input: Option         |
+---------------------------+
        |
        v
+---------------------------+    +---------------------------+    +---------------------------+
| Option 1: Add Product      | ->| Option 2: Remove Product   | ->| Option 3: Display Inventory|
+---------------------------+    +---------------------------+    +---------------------------+
        |
        v
+---------------------------+
| Option 4: Exit             |
+---------------------------+
        |
        v
+---------------------------+
| End                       |
+---------------------------+
```

### 3. System Architecture

This diagram shows how the classes interact during runtime.

```
+---------------------------+
|         User               |
+---------------------------+
            |
            v
+---------------------------+
|        Inventory           |
+---------------------------+
            |
            v
+-----------------------------------+
|           Product                 |
|-----------------------------------|
|  + addProduct() : void            |
|  + removeProduct() : bool         |
+-----------------------------------+
      /                \
     /                  \
+-------------------+  +-------------------+
| PerishableProduct |  | NonPerishableProd |
+-------------------+  +-------------------+
```

---

## Future Enhancements

1. **Customer Management**: Implement a `Customer` class to store customer details and track their purchase history.
2. **File I/O**: Enable saving and loading of inventory data to/from a file for persistent storage.
3. **Enhanced Search**: Add a feature to search for products by name or category.
