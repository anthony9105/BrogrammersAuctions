#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include "user/user.h"
#include "transactions/transaction.h"
#include "transactions/delete.h"
#include "transactions/create.h"
#include "transactions/addcredit.h"
using namespace std;

/// @brief logIn function used to login
/// @param username - username of user trying to login
/// @return - vector<string> containing information about the user
vector<string> logIn(string username) {
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            if (result[i] == username) {
                cout << "Login successful" << endl;
                isLoggedIn = true;
                readUserAccountsFile.close();
                return result;
            }
        }
    }

    readUserAccountsFile.close();
    cout << "Error. Login unsuccessful.  No account corresponding to this username" << endl;
    return vector<string>();
}


int main() {
    string line;
    vector<string> userInput;
    vector<string> userInfo;
    Transaction transactionSession;

    do {
        getline(cin, line);
        userInput = splitIntoVector(line);

        // when login is entered
        if (userInput[0] == "login") {
            if (isLoggedIn) {
                cout << "Error.  You are already logged in." << endl;
                break;
            }
            userInfo = logIn(userInput[1]);

            if (userInfo.empty()) {
                userInput[0] == "close";
            }
            else {
                // set transactionSession with the informaton returned from logIn function
                transactionSession.setName(userInfo[0]);
                transactionSession.setAccountType(userInfo[1]);
                transactionSession.setBalance(stoi(userInfo[2]));
            }
        }
        // when create is entered
        else if (userInput[0] == "create") {
            Create createTransaction;
            createTransaction.executeTransaction();
        }
        // commands that require te user to be logged in
        else {
            // when not logged in
            if (!isLoggedIn) {
                cout << "Error. You are not logged in" << endl;
            }  
            else { 
                if (userInput[0] == "logout") {
                    if (!isLoggedIn) {
                        cout << "Error.  You cannot logout if you are not already logged in." << endl;
                    }
                    else {
                        isLoggedIn = false;
                        cout << "logout successful" << endl;
                    }
                }
                // when delete is entered
                else if (userInput[0] == "delete") {
                    if (transactionSession.getAccountType() == "AA") {
                        Delete deleteTransaction;
                        deleteTransaction.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
                    }
                    else {
                        cout << "You can not delete any accounts unless you are on an admin account" << endl;
                    }
                }
                // todo
                else if (userInput[0] == "advertise") {}
                // todo
                else if (userInput[0] == "bid") {}
                // todo
                else if (userInput[0] == "refund") {}
                // when addcredit is entered
                else if (userInput[0] == "addcredit") {
                    AddCredit addCreditTransaction;
                    addCreditTransaction.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
                }
                // unknown/invalid command
                else {
                    cout << "Unknown command.  Try again" << endl;
                }
            }
        }
    } while (userInput.empty() || userInput[0] != "close");

    return 0;
}