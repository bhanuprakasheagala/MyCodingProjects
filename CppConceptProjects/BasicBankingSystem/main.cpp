#include "BankAccount.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<BankAccount> accounts; // All Bank Accounts

    while(true) {
        std::cout << "\nSimple Banking System\n";
        std::cout << "1. Create Account\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. Display Account Information\n";
        std::cout << "5. Display Transaction History\n";
        std::cout << "6. Calculate Interest (Savings Accounts Only)\n";
        std::cout << "7. Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if(choice == 1) {
            // Create Account
            int accNumber;
            std::string accHolderName;
            double initialBalance;
            std::string accType;
            std::string password;

            std::cout << "Enter Account Number: ";
            std::cin >> accNumber;

            std::cout << "Enter Account Holder Name: ";
            std::cin.ignore(); // To clear the input buffer
            std::getline(std::cin, accHolderName);

            std::cout << "Enter Initial Balance: ";
            std::cin >> initialBalance;

            std::cout << "Enter Account Type (Savings/Checking): ";
            std::cin >> accType;

            std::cout << "Set Account Password: ";
            std::cin >> password;

            accounts.emplace_back(accNumber, accHolderName, initialBalance, accType, password);
            std::cout << "Account created successfully!!\n";
        }
        else if(choice == 2 || choice == 3 || choice == 4) {
            // Authentication before deposit, withdrawal, or viewing account

            int accNumber;
            
            std::string password;
            
            std::cout << "Enter Account Number: ";
            std::cin >> accNumber;

            bool found = false;
            for(auto& account : accounts) {
                if(account.getAccountNumber() == accNumber) {
                    std::cout << "Enter Password: ";
                    std::cin >> password;

                    if(account.validatePassword(password)) {
                        found = true;

                        // Perform the corresponding operation
                        if(choice == 2) {
                            double amount;
                            std::cout << "Enter Deposit Number: ";
                            std::cin >> amount;
                            account.deposit(amount);
                        }
                        else if(choice == 3) {
                            // Withdraw money
                            double amount;
                            std::cout << "Enter Withdrawal Amount: ";
                            std::cin >> amount;
                            account.withdraw(amount);
                        }
                        else if(choice == 4) {
                            // Display account information
                            account.displayAccountInfo();
                        }
                        break;
                    }
                    else {
                        std::cout << "Invalid Password!\n";
                        found = true;
                        break;
                    }
                }
            }
            if(!found) {
                std::cout << "Account not found!\n";
            }
        }
        else if(choice == 5) {
            // Display Transaction History
            int accNumber;
            std::cout << "Enter Account Number: ";
            std::cin >> accNumber;

            bool found = false;
            for(const auto& account : accounts) {
                if(account.getAccountNumber() == accNumber) {
                    account.displayTransactionHistory();
                    found = true;
                    break;
                }
            }
            if(!found) {
                std::cout << "Account Not Found!\n";
            }
        }
        else if(choice == 6) {
            // Calculate Interest 
            int accNumber;
            double interestRate;

            std::cout << "Enter Account Number: ";
            std::cin >> accNumber;

            std::cout << "Enter Interest Rate (%): ";
            std::cin >> interestRate;

            bool found = false;
            for(auto& account : accounts) {
                if(account.getAccountNumber() == accNumber) {
                    account.calculateInterest(interestRate);
                    found = true;
                    break;
                }
            }
            if(!found) {
                std::cout << "Account Not Found!\n";
            }
        }
        else if(choice == 7) {
            // Exit the program
            std::cout << "Exiting the program. Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}