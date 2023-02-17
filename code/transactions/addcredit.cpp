#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

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
            usernameExists = Transaction::checkIfUserExists(nameToAddCredit);

            if (!usernameExists) {
                cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
            }
        }
    }

    int creditToAdd;
    cout << "Enter credit to add to user" << endl;
    cin >> creditToAdd;

    // use functions in the parent class Transaction
    Transaction::updateCreditInUsersFile(nameToAddCredit, creditToAdd);
    Transaction::addToTransFile(name, accountType, balance, "06");
    cout << "Add credit successful" << endl;

    // seg fault after here for some reason
}