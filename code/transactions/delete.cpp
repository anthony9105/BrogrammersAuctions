#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include "../user/user.h"
#include "transaction.h"
#include "delete.h"

/**
 * Delete class
*/
void Delete::executeTransaction(string name, string accountType, int balance) {
    string nameToDelete;
    bool usernameExists = false;

    cout << "Enter username of user to delete" << endl;

    // while loop that does not end until valid input is entered
    while (!usernameExists) {
        getline(cin, nameToDelete);

        // to exit from/cancel delete entirely
        if (Transaction::cancelTransaction(nameToDelete)) {
            return;
        }

        usernameExists = Transaction::checkIfUserExists(nameToDelete);

        if (!usernameExists) {
            cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
        }
    }

    // use functions in the parent class Transaction
    Transaction::removeFromUsersFile(nameToDelete);
    Transaction::addToTransFile(name, accountType, balance, "02");
    cout << "User account deleted" << endl;
}