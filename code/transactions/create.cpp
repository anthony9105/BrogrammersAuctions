#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include "../user/user.h"
#include "transaction.h"
#include "create.h"

/**
 * Create class
*/
void Create::executeTransaction() {
    bool usernameIsInUse = true;
    bool nameTooLong = true;
    bool validAccountType = false;
    string username, accountType;
    
    cout << "Enter new username" << endl;

    // while loop that does not end until valid input is entered
    while (usernameIsInUse || nameTooLong) {
        getline(cin, username);
        usernameIsInUse = Transaction::checkIfUserExists(username);
        nameTooLong = Transaction::nameIsTooLong(username);

        if (usernameIsInUse) {
            cout << "Error.  Username is already in use.  Enter a new username" << endl;
        }

        if (nameTooLong) {
            cout << "Error.  Username is too long.  Enter a new username" << endl;
        }
    }

    cout << "Enter type of user (admin (AA), full-standard (FS), buy-standard (BS), or sell-standard (SS))" << endl;
    // while loop that does not end until valid input is entered
    while (!validAccountType) {
        getline(cin, accountType);
        if (accountType == "AA" || accountType == "FS" || accountType == "BS" || accountType == "SS") {
            validAccountType = true;
        }
        cout << "Please enter a valid type of user (admin (AA), full-standard (FS), buy-standard (BS), or sell-standard (SS))" << endl;
    }

    // use functions in the parent class Transaction
    Transaction::addToUsersFile(username, accountType, balance);
    Transaction::addToTransFile(username, accountType, balance, "01");

    cout << "Success.  New user added." << endl;
}