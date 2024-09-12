#include "BankAccount.hpp"

// Constructor to initialize the Account details
BankAccount::BankAccount(int accNumber, const std::string& accHolderName, double initialBalance, const std::string& accType, const std::string& pass)
    : accountNumber(accNumber), accountHolderName(accHolderName), balance(initialBalance), accountType(accType), password(pass) {}

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

// Method to display account information
void BankAccount::displayAccountInfo() const {
    std::cout << "Account Number: " << accountNumber << '\n';
    std::cout << "Account Holder: " << accountHolderName << '\n';
    std::cout << "Account Type: " << accountType << '\n';
    std::cout << "Balance: Rupees " << balance << '\n';
}

// Method to display transaction history
void BankAccount::displayTransactionHistory() const {
    std::cout << "Transaction History for Account " << accountNumber << '\n';
    for(const auto& transaction : transactionHistory) {
        std::cout << transaction.type << ": INR " << transaction.amount << '\n';
    }
}

// Method to calculate interest rate (Only for Savings account)
void BankAccount::calculateInterest(double interestRate) {
    if(accountType == "Savings") {
        double interest = balance * (interestRate / 100);
        deposit(interest); // Add interest to the balance
        std::cout << "Interest of INR " << interest << " added to the account.\n";
    }
    else {
        std::cout << "Interest calculation is only available for Savings account\n";
    }
}

// Method to validate the entered password
bool BankAccount::validatePassword(const std::string& enteredPassword) const {
    return enteredPassword == password;
}

// Getter for account number
int BankAccount::getAccountNumber() const {
    return accountNumber;
}

// Getter for account type
std::string BankAccount::getAccountType() const {
    return accountType;
}