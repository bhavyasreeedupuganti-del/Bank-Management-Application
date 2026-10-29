#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cstdio>
#include <algorithm>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    string holderName;
    double balance;

public:
    BankAccount() : accountNumber(0), balance(0.0) {}

    BankAccount(int number, string name, double amount)
        : accountNumber(number), holderName(name), balance(amount) {}

    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }

    bool deposit(double amount) {
        if (!isfinite(amount) || amount <= 0 ||
            !isfinite(balance + amount))
            return false;

        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (!isfinite(amount) || amount <= 0 ||
            amount > balance)
            return false;

        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\nAccount Number: " << accountNumber
             << "\nAccount Holder: " << holderName
             << "\nBalance: Rs. " << fixed << setprecision(2)
             << balance << '\n';
    }

    string toFileLine() const {
        ostringstream out;
        out << accountNumber << '|' << holderName << '|'
            << fixed << setprecision(2) << balance;
        return out.str();
    }
};

class BankManagementSystem {
private:
    const string filename = "customers.txt";

    bool findAccount(int number, BankAccount& account) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string numberText, name, balanceText;

            if (!getline(ss, numberText, '|') ||
                !getline(ss, name, '|') ||
                !getline(ss, balanceText))
                continue;

            try {
                int savedNumber = stoi(numberText);
                double savedBalance = stod(balanceText);

                if (savedNumber == number &&
                    isfinite(savedBalance) && savedBalance >= 0) {
                    account = BankAccount(
                        savedNumber, name, savedBalance);
                    return true;
                }
            } catch (...) {
                continue;
            }
        }
        return false;
    }

    int generateAccountNumber() {
        ifstream file(filename);
        string line;
        int highest = 1000;

        while (getline(file, line)) {
            stringstream ss(line);
            string numberText;

            if (getline(ss, numberText, '|')) {
                try {
                    highest = max(highest, stoi(numberText));
                } catch (...) {
                    continue;
                }
            }
        }
        return highest + 1;
    }

    bool saveAccount(const BankAccount& account) {
        ifstream input(filename);
        ofstream temp("customers.tmp");

        if (!temp) return false;

        string line;
        bool replaced = false;

        while (getline(input, line)) {
            stringstream ss(line);
            string numberText;

            if (getline(ss, numberText, '|')) {
                try {
                    if (stoi(numberText) == account.getAccountNumber()) {
                        temp << account.toFileLine() << '\n';
                        replaced = true;
                        continue;
                    }
                } catch (...) {
                    // Keep any malformed existing record.
                }
            }
            temp << line << '\n';
        }

        if (!replaced)
            temp << account.toFileLine() << '\n';

        temp.close();
        input.close();

        if (!temp) {
            remove("customers.tmp");
            return false;
        }

        if (rename("customers.tmp", filename.c_str()) != 0) {
            remove(filename.c_str());
            if (rename("customers.tmp", filename.c_str()) != 0)
                return false;
        }

        return true;
    }

    bool readAccountNumber(int& number) {
        cout << "Enter account number: ";

        if (!(cin >> number)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid account number.\n";
            return false;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }

    bool readAmount(double& amount) {
        if (!(cin >> amount)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount.\n";
            return false;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!isfinite(amount) || amount <= 0) {
            cout << "Amount must be greater than zero.\n";
            return false;
        }

        return true;
    }

public:
    void createAccount() {
        string name;
        double initialBalance;

        cout << "Enter account holder's name: ";
        getline(cin >> ws, name);

        if (name.empty() || name.find('|') != string::npos) {
            cout << "Invalid name. Please try again.\n";
            return;
        }

        cout << "Enter initial deposit (0 or more): Rs. ";

        if (!(cin >> initialBalance)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!isfinite(initialBalance) || initialBalance < 0) {
            cout << "Initial deposit cannot be negative.\n";
            return;
        }

        BankAccount account(
            generateAccountNumber(), name, initialBalance);

        if (saveAccount(account)) {
            cout << "\nAccount created successfully!\n";
            account.display();
        } else {
            cout << "Error saving account.\n";
        }
    }

    void depositMoney() {
        int number;
        if (!readAccountNumber(number)) return;

        BankAccount account;
        if (!findAccount(number, account)) {
            cout << "Account not found.\n";
            return;
        }

        double amount;
        cout << "Enter deposit amount: Rs. ";
        if (!readAmount(amount)) return;

        if (!account.deposit(amount)) {
            cout << "Invalid amount or balance limit exceeded.\n";
            return;
        }

        if (!saveAccount(account)) {
            cout << "Could not save the updated balance.\n";
            return;
        }

        cout << "Deposit successful!\n";
        account.display();
    }

    void withdrawMoney() {
        int number;
        if (!readAccountNumber(number)) return;

        BankAccount account;
        if (!findAccount(number, account)) {
            cout << "Account not found.\n";
            return;
        }

        double amount;
        cout << "Enter withdrawal amount: Rs. ";
        if (!readAmount(amount)) return;

        if (amount > account.getBalance()) {
            cout << "Insufficient balance!\n";
            return;
        }

        account.withdraw(amount);

        if (!saveAccount(account)) {
            cout << "Could not save the updated balance.\n";
            return;
        }

        cout << "Withdrawal successful!\n";
        account.display();
    }

    void checkBalance() {
        int number;
        if (!readAccountNumber(number)) return;

        BankAccount account;
        if (findAccount(number, account))
            account.display();
        else
            cout << "Account not found.\n";
    }

    void displayAllAccounts() {
        ifstream file(filename);
        string line;
        bool found = false;

        cout << "\n====== CUSTOMER ACCOUNTS ======\n";

        while (getline(file, line)) {
            stringstream ss(line);
            string number, name, balance;

            if (getline(ss, number, '|') &&
                getline(ss, name, '|') &&
                getline(ss, balance)) {
                cout << "Account: " << number
                     << " | Name: " << name
                     << " | Balance: Rs. " << balance << '\n';
                found = true;
            }
        }

        if (!found)
            cout << "No customer records found.\n";
    }

    void run() {
        int choice;

        do {
            cout << "\n=================================\n"
                 << "     BANK MANAGEMENT SYSTEM\n"
                 << "=================================\n"
                 << "1. Create Account\n"
                 << "2. Deposit Money\n"
                 << "3. Withdraw Money\n"
                 << "4. Check Balance\n"
                 << "5. Display All Accounts\n"
                 << "0. Exit\n"
                 << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid menu number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: createAccount(); break;
                case 2: depositMoney(); break;
                case 3: withdrawMoney(); break;
                case 4: checkBalance(); break;
                case 5: displayAllAccounts(); break;
                case 0:
                    cout << "Thank you for using our bank system!\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    BankManagementSystem bank;
    bank.run();
    return 0;
}