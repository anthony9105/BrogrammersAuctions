#include "advertise.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "transaction.h"
#include "../user/user.h"

const string ADVERTISE_TRANS_CODE = "03";

void Advertise::executeTransaction(string name, string accountType, double balance, string availItemsFile) {

    // only for admins, sell-standard, or full-standard users (not buy-standard)
    if (accountType == "AA" || accountType == "SS" || accountType == "FS") {
        string itemName;
        bool itemAlreadyExists = true;
        bool validItemName = false;

        // while loop to run until an item name is entered that is valid
        while (itemAlreadyExists || !validItemName) {
            cout << "Enter a new item name (max 25 characters)" << endl;
            cin >> itemName;

            // check if user wants to cancel transaction
            if (Transaction::cancelTransaction(itemName)) {
                return;
            }

            itemAlreadyExists = Transaction::checkIfExists(itemName, availItemsFile);

            // if item name is less than 26 characters and doesn't already exist
            if (itemName.length() <= 25 && !itemAlreadyExists) {
                validItemName = true;
            }
        }

        string bidResponse;
        double minimumBid;
        bool validMinimumBid = false;

        // while loop running until the minimum bid entered is valid
        while(!validMinimumBid) {
            try {
                cout << "Enter a valid minimum bid (max: 999.99)" << endl;
                cin >> bidResponse;
                minimumBid = stod(bidResponse);
                
                 // if the response is less than 4 characters (max is 100) and last character is a digit
                if (daysResponse.length() < 4 && isdigit(daysResponse[daysResponse.length() - 1])) {
                    // if under 999.99 and over or equal to 0.01
                    if (minimumBid <= 999.99 && minimumBid >= 0.01) {
                        // if 2 or less decimals
                        if (is2OrLessDecimals(bidResponse)) {
                            validMinimumBid = true;
                        }
                    }
                }
            }
            catch (invalid_argument& ia) {
                // check if user wants to cancel transaction
                if (Transaction::cancelTransaction(bidResponse)) {
                    return;
                }

                cin.clear();
            }
        } 

        string daysResponse;
        int numOfDays;
        bool validNumOfDays = false;

        // while loop to run until a valid number of days remaining for auction on the item is entered
        while (!validNumOfDays) {
            cout << "Enter number days remaining to bid on this item (max: 100 days)" << endl;
            try {
                cin >> daysResponse;
                numOfDays = stoi(daysResponse);

                // if the response is less than 4 characters (max is 100) and last character is a digit
                if (daysResponse.length() < 4 && isdigit(daysResponse[daysResponse.length() - 1])) {
                    if (numOfDays <= 100) {
                        validNumOfDays = true;
                    }   
                }
            }
            catch (invalid_argument& ia) {
                // check if user wants to cancel transaction
                if (Transaction::cancelTransaction(daysResponse)) {
                    return;
                }

                cin.clear();
            }
        }

        // add to the appropriate files
        Transaction::addToItemsFile(itemName, name, numOfDays, minimumBid);
        Transaction::addToTransFile(name, accountType, balance, ADVERTISE_TRANS_CODE);

        cout << "Item successfully added for auction" << endl;
        return;
    }
    else {
        cout << "You do not have the account privileges to do this" << endl;
        return;
    }

}
