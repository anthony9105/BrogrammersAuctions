#include "bid.h"
#include "../user/user.h"
#include "addcredit.h"
#include <iostream>
#include <fstream>

using namespace std;

Bid::Bid() {
    // set files used by this transaction
    setFiles(DAILY_TRANS_FILE, CURR_USER_ACC_FILE);
}

void Bid::executeTransaction() {
    vector<string> userInput;
    string item, seller, highestBidder;
    int currentHighestBid, bidAmount;

    // prompt user for item and seller name
    cout << "Enter the name of the item you want to bid on: ";
    getline(cin, item);
    cout << "Enter the seller name: ";
    getline(cin, seller);

    // read item file
    ifstream readItemFile(item + ".txt");
    if (!readItemFile) {
        cout << "Error. Item not found." << endl;
        return;
    }

    // extract information from item file
    readItemFile >> seller >> currentHighestBid >> highestBidder;
    readItemFile.close();

    // check if the user already made a bid on this item
    ifstream readDailyTransFile(getFiles()[0]);
    string line;
    bool alreadyBidOnItem = false;
    while (getline(readDailyTransFile, line)) {
        vector<string> tokens = splitIntoVector(line);
        if (tokens[0] == "10" && tokens[2] == item && tokens[3] == getUser().getUsername()) {
            alreadyBidOnItem = true;
            break;
        }
    }
    readDailyTransFile.close();
    if (alreadyBidOnItem) {
        cout << "Error. You have already made a bid on this item." << endl;
        return;
    }

    // prompt user for bid amount
    cout << "Enter the bid amount: ";
    cin >> bidAmount;
    cin.ignore(); // ignore the newline character in the input stream

    // check if bid is higher than current highest bid
    if (bidAmount <= currentHighestBid) {
        cout << "Error. Your bid amount must be greater than the current highest bid of $" << currentHighestBid << endl;
        return;
    }

    // add credit transaction to update user balance
    AddCredit addCreditTransaction;
    addCreditTransaction.setFiles(getFiles()[0], getFiles()[1]);
    addCreditTransaction.executeTransaction(getUser().getUsername(), getUser().getAccountType(), getUser().getBalance(), to_string(-1 * bidAmount));

    // update item file with new highest bid and bidder
    ofstream writeItemFile(item + ".txt");
    writeItemFile << seller << " " << bidAmount << " " << getUser().getUsername();
    writeItemFile.close();

    // add bid transaction to daily transaction file
    addToTransFile(getUser().getUsername(), getUser().getAccountType(), getUser().getBalance(), "10 " + item + " " + to_string(bidAmount));

    cout << "Bid successful." << endl;
}
