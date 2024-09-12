#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <iostream>
#include <string>
#include <vector>

// Transaction structure to store transaction details
struct Transaction {
    std::string type; // "Withdrawal" or "Deposit"
    double amount;
};

// BankAccount class definition
class BankAccount {
    private:
        int accountNumber;
        std::string accountHolderName;
        double balance;
        std::string accountType; // "Savings" or "Checking"
        std::vector<Transaction> transactionHistory;
        std::string password;

    public:
        // Constructor to initialize account details
        BankAccount(int accNumber, const std::string& accHolderName, double initialBalance, const std::string& accType, const std::string& pass);

        // Method to deposit money into the account
        void deposit(double amount);

        // Method to withdraw money from the account
        bool withdraw(double amount);

        // Method to display account information
        void displayAccountInfo() const;

        // Method to display transaction history
        void displayTransactionHistory() const;

        // Method to calculate interest(Only for Savings account)
        void calculateInterest(double interestRate);

        // Getter for account number
        int getAccountNumber() const;

        // Getter for account type
        std::string getAccountType() const;

        // Method to validate user password
        bool validatePassword(const std::string& enteredPassword) const;
};

#endif