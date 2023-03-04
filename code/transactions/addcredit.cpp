#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>

#include "../user/user.h"
#include "transaction.h"
#include "addcredit.h"

void AddCredit::executeTransaction(string name, string accountType, int balance) {
    string nameToAddCredit;
    nameToAddCredit = name;
    bool usernameExists = false;

    if (accountType == "AA") {
        cout << "Enter username of account to add credit to" << endl;

        // while loop that does not end until valid input is entered        
        while (!usernameExists) {
            getline(cin, nameToAddCredit);

            // to exit from/cancel addcredit entirely
            if (Transaction::cancelTransaction(nameToAddCredit)) {
                return;
            }

            usernameExists = Transaction::checkIfUserExists(nameToAddCredit);

            if (!usernameExists) {
                cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
            }

        }
    }

    int creditToAdd;
    bool validCreditToAdd = false;
    int balanceOfUserToAddTo = Transaction::getBalanceFromChosenUser(nameToAddCredit);

    if (balanceOfUserToAddTo >= 999999999) {
        cout << "Balance is already at the maximum of 999999999" << endl;
        return;
    }

    // loop to run until the credit added is a valid amount
    while (!validCreditToAdd) {
        cout << "Enter credit to add to user" << endl;

        cin.exceptions(ios_base::failbit);

        try {
            cin >> creditToAdd;

            if (balanceOfUserToAddTo + creditToAdd > 999999999) {
                cout << "Error. This transaction will cause credit account to exceed max of 999999999" << endl;
            }
            else if (Transaction::sessionCreditLimitExceeded(creditToAdd)) {
                cout << "Error. No more than $1000.00 can be added in a given session" << endl;
            }
            else {
                validCreditToAdd = true;
                break;
            }
        }
        // invalid input such as entering a string instead of int
        catch (ios_base::failure &) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // if the user is adding credit to themselves
    if (nameToAddCredit == name) {
        balance += creditToAdd;
    }

    // use functions in the parent class Transaction
    Transaction::updateCreditInUsersFile(nameToAddCredit, creditToAdd);
    Transaction::addToTransFile(name, accountType, balance, "06");
    cout << "Add credit successful" << endl;
    return;

    // seg fault after here for some reason
}