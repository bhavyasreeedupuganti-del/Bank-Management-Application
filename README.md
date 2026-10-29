# Bank Management Application

## Project Overview

The Bank Management Application is a menu-driven C++ project designed to simulate basic banking operations. It uses object-oriented programming concepts and file handling to maintain customer account information even after the program is closed.

## Objectives

- To implement basic banking operations using C++.
- To understand classes, objects, and encapsulation.
- To store and retrieve customer records using file handling.
- To perform deposits, withdrawals, and balance inquiries.
- To validate user input and prevent invalid transactions.

## Features

1. **Create Account:** Creates a customer account with a unique account number.
2. **Deposit Money:** Adds money to an existing account.
3. **Withdraw Money:** Withdraws money after checking the available balance.
4. **Check Balance:** Displays account details and the current balance.
5. **Display All Accounts:** Shows saved customer records.
6. **Persistent Storage:** Saves account information in a text file.

## Technologies Used

- Programming Language: C++
- Concepts: Object-Oriented Programming (OOP)
- File Handling: C++ file streams
- Development Environment: Visual Studio Code

## OOP Concepts

- **Classes and Objects:** Used to represent bank accounts and the banking system.
- **Encapsulation:** Account information is stored in private data members.
- **Abstraction:** Banking operations are provided through public methods.

## How to Run

1. Install a C++ compiler such as GCC or MinGW.
2. Open the project folder in Visual Studio Code.
3. Open the integrated terminal.
4. Compile the program:

   `g++ -std=c++17 main.cpp -o bank`

5. Run the program on Windows:

   `.\bank.exe`

6. Follow the menu displayed in the terminal.

## Data Storage

Customer account records are stored in `customers.txt`. The file is updated when accounts are created or transactions are completed. This allows records to remain available when the application is restarted.

## Expected Outcome

A functional console-based banking application that demonstrates core banking operations, object-oriented programming, input validation, and persistent file storage.

## Limitations

This project is intended for educational purposes only. Customer records are stored in an unencrypted text file, and the application does not implement authentication or production-grade security. It must not be used to manage real financial accounts.

## Author

Developed as a C++ programming project to practice object-oriented programming and file handling.
