#include "advertise.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "transaction.h"
#include "../user/user.h"

void Advertise::executeTransaction(string name, string accountType) {

    if (accountType = "AA" || accountType == "SS" || accountType "FS") {
        string itemName;
        bool itemAlreadyExists = true;

        while (itemAlreadyExists) {
            cout << "Enter a new item name (max 25 characters)" << endl;
            cin >> itemName;
            itemIsValid = Transaction::checkIfExists(itemName);
        }

        double minimumBid;
        bool validMinimumBid = false;

        while(!validMinimumBid) {
            cout << "Enter a valid minimum bid (max: 999.99)" << endl;
            cin >> minimumBid;
            
            if (minimumBid <= 999.99 && minimumBid > 0.01) {
                // get the number of decimal places
                double temp = minimumBid;
                int len = temp.find_last_of('.');
                temp.erase(0,len+1);
                len = temp.length();
                
                // if the decimal places is only 2 or less
                if (len <= 2) {
                    validMinimumBid = true;
                }
            }
        } 

        int numOfDays;
        bool validNumOfDays = false;

        while (!validNumOfDays) {
            cout << "Enter number days remaining to bid on this item (max: 100 days)" << endl;
            cin >> numOfDays;

            if
        }

    }
    else {
        cout << "You do not have the account privileges to do this" << endl;
        return;
    }

}
