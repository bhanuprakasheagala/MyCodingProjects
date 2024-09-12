# Simple Banking System

## Description

The **Simple Banking System** is a C++ console-based application that allows users to simulate banking operations like creating accounts, depositing and withdrawing money, checking account balances, and maintaining transaction history. It uses basic OOP concepts like classes, objects, and member functions, making it a great project for understanding core C++ concepts.

This project also includes an authentication feature where users must log in using their account number and password before performing sensitive operations. The system supports both `Savings` and `Checking` accounts and includes interest calculation for `Savings` accounts.

## Features

1. **Create Accounts**: Users can create `Savings` or `Checking` accounts by entering details like account number, holder name, initial balance, and a password.
2. **Deposit Money**: Users can deposit money into their account after authenticating with their password.
3. **Withdraw Money**: Users can withdraw money after successful authentication, provided they have sufficient funds.
4. **Display Account Information**: Shows account details like account number, holder name, account type, and current balance.
5. **Transaction History**: The system records and displays a list of all deposits and withdrawals for each account.
6. **Interest Calculation**: Interest can be calculated and added to `Savings` accounts.
7. **User Authentication**: A password is required to perform operations on an account, enhancing security.

## File Structure

```
SimpleBankingSystem/
├── BankAccount.hpp        # Header file defining the BankAccount class and methods
├── BankAccount.cpp        # Implementation file for the BankAccount class methods
├── main.cpp               # Main program that runs the banking system and handles user input
└── README.md              # Project documentation (this file)
```

### Code Breakdown

- **`BankAccount.hpp`**: This file contains the declaration of the `BankAccount` class. It defines the attributes such as account number, account holder's name, balance, account type, password, and transaction history. Key methods include `deposit()`, `withdraw()`, `calculateInterest()`, and `validatePassword()`.
  
- **`BankAccount.cpp`**: The implementation of all the methods declared in the header file. This file handles operations like modifying the balance, updating the transaction history, validating passwords, and calculating interest for savings accounts.
  
- **`main.cpp`**: This file contains the user interface logic. It presents a simple console menu for the user to create accounts, perform banking operations, and interact with the system. It manages user input and handles authentication before allowing operations on accounts.

## How to Run

### Prerequisites

- **C++ Compiler**: You need a C++ compiler that supports at least C++11. Examples:
  - GCC (GNU Compiler Collection)
  - MSVC (Microsoft Visual C++)
  - Clang

### Compiling and Running the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/SimpleBankingSystem.git
   ```

2. Navigate to the project directory:

   ```bash
   cd SimpleBankingSystem
   ```

3. Compile the project:

   ```bash
   g++ main.cpp BankAccount.cpp -o banking_system
   ```

4. Run the compiled executable:

   ```bash
   ./banking_system
   ```

### Sample Program Output

When you run the program, you'll see a menu like this:

```
Simple Banking System
1. Create Account
2. Deposit Money
3. Withdraw Money
4. Display Account Information
5. Display Transaction History
6. Calculate Interest (Savings Accounts Only)
7. Exit
Choose an option:
```

- After selecting an option, the program will prompt you for additional information, such as account number, deposit/withdrawal amount, or password, depending on the operation.

### Authentication

Each time you attempt to deposit, withdraw, or view account information, you'll be asked to provide your password. Only if the password matches will you be able to proceed with the action. This enhances security for users.

### Account Types and Interest

- **Savings Account**: You can apply interest to the balance by selecting the interest calculation option.
- **Checking Account**: No interest is calculated for checking accounts.

## Improvements

- Add a **file I/O** feature to persist accounts and transactions between program runs.
- Implement a **graphical user interface (GUI)** for a better user experience.
- Add **more account types**, like Business accounts with different features.
