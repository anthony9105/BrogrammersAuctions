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

const string ADDCREDIT_TRANS_CODE = "06";

double AddCredit::executeTransaction(string name, string accountType, double balance, string userFilePath) {
    string nameToAddCredit;
    nameToAddCredit = name;
    bool usernameExists = false;
    bool personIsAddingCreditToThemself = false;

    // if the user doing this transaction is an admin
    if (accountType == "AA") {
        cout << "Enter username of account to add credit to" << endl;

        // while loop that does not end until valid input is entered        
        while (!usernameExists) {
            getline(cin, nameToAddCredit);

            // to exit from/cancel addcredit entirely
            if (Transaction::cancelTransaction(nameToAddCredit)) {
                return -1.0;
            }

            // check if the username exists in the CurrentUserAccounts.txt file
            usernameExists = Transaction::checkIfExists(nameToAddCredit, userFilePath);

            if (!usernameExists) {
                cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
            }

        }
    }

    if (name == nameToAddCredit) {
        personIsAddingCreditToThemself = true;
    }

    double creditToAdd;
    string response;
    bool validCreditToAdd = false;
    double balanceOfUserToAddTo = Transaction::getBalanceFromChosenUser(nameToAddCredit);

    // if the balance of the user to add to is already at the maximum credit
    if (balanceOfUserToAddTo >= 999999999.99) {
        cout << "Balance is already at the maximum of 999999999.99" << endl;
        return -1.0;
    }

    // loop to run until the credit added is a valid amount
    while (!validCreditToAdd) {
        cout << "Enter credit to add to user" << endl;

        try {
            getline(cin, response);
            creditToAdd = stod(response);

            if (balanceOfUserToAddTo + creditToAdd > 999999999.99) {
                cout << "Error. This transaction will cause credit account to exceed max of 999999999" << endl;
            }
            else if (Transaction::sessionCreditLimitExceeded(creditToAdd)) {
                cout << "Error. No more than $1000.00 can be added in a given session" << endl;
            }
            else {
                if (is2OrLessDecimals(response)) {
                    validCreditToAdd = true;
                }
            }
        }
        // invalid input such as entering a string instead of int
        catch (invalid_argument& ia) {
            // check if user wants to cancel transaction
            if (Transaction::cancelTransaction(response)) {
                return -1.0;
            }
            cout << "Invalid input" << endl;
            cin.clear();
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (exception& e) {
            cout << "fdfeihfdiehfie" << endl;
        }
    }

    // if the user is adding credit to themselves
    // if (nameToAddCredit == name) {
    //     balance += creditToAdd;
    // }
    balanceOfUserToAddTo += creditToAdd;

    // use functions in the parent class Transaction
    Transaction::updateCreditInUsersFile(nameToAddCredit, creditToAdd);

    if (personIsAddingCreditToThemself) {
        Transaction::addToTransFile(name, accountType, balanceOfUserToAddTo, ADDCREDIT_TRANS_CODE);
        cout << "Add credit successful" << endl;
        return balanceOfUserToAddTo;
    }    
    else { 
        Transaction::addToTransFile(name, accountType, balance, ADDCREDIT_TRANS_CODE);
        cout << "Add credit successful" << endl;
        return 0.0;
    }


}