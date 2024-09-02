#include "BankAccount.hpp"

// Constructor to initialize the Account details
BankAccount::BankAccount(int accNumber, const std::string& accHolderName, double initialBalance, const std::string& accType)
    : accountNumber(accNumber), accountHolderName(accHolderName), balance(initialBalance), accountType(accType) {}

// Method to deposit money into the account
void BankAccount::deposit(double amount) {
    if(amount > 0) {
        balance += amount;
        transactionHistory.push_back({"Deposit", amount});
        std::cout << "Deposited: Rupees " << amount << '\n';
    }
    else {
        std::cout << "Invalid deposit amount!\n";
    }
}

// Method to withdraw money from the account
bool BankAccount::withdraw(double amount) {
    if(amount > 0 && amount <= balance) {
        balance -= amount;
        transactionHistory.push_back({"Withdrawal", amount});
        std::cout << "Withdrawn: Rupees " << amount << '\n';
        return true;
    }
    else {
        std::cout << "Invalid withdrawal amount or Insufficient balance.\n";
        return false;
    }
}