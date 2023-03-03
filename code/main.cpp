#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <thread>

#include "user/user.h"
#include "transactions/transaction.h"
#include "transactions/delete.h"
#include "transactions/create.h"
#include "transactions/addcredit.h"
using namespace std;

Transaction transactionSession; 


/// @brief logIn function used to login
/// @param username - username of user trying to login
/// @return - vector<string> containing information about the user
vector<string> logIn(string username, string submittedPassword) {
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            if (result[i] == username) {
                //cout << "Login successful" << endl;
                //isLoggedIn = true;
                readUserAccountsFile.close();
                break;
                //return result;
            }
        }
    }

    //readUserAccountsFile.close();
    bool usernameValid, passwordCorrect;

    usernameValid = transactionSession.checkIfUserExists(username);
    passwordCorrect = transactionSession.passwordAccepted(submittedPassword);
    if (!usernameValid) {
        cout << "Error. Login unsuccessful.  No account corresponding to this username" << endl;
        return vector<string>();
    }
    else if (!passwordCorrect) {
        cout << "Error. Login unsuccessful.  Password incorect" << endl;
        return vector<string>();
    }
    else {
        cout << "Login successful" << endl;
        isLoggedIn = true;
        return result;
    }
}

void runSession()
{
    string line;
    vector<string> userInput;
    vector<string> userInfo;


    do {
        getline(cin, line);
        userInput = splitIntoVector(line);

        if (userInput[0] == "close") {
            return;
        }

        // when login is entered
        if (userInput[0] == "login") {
            if (isLoggedIn) {
                cout << "Error.  You are already logged in." << endl;
                break;
            }

            if (userInput.size() < 3) {
                cout << "Please enter a password" << endl;
                userInput[0] = "";
                //break;
            }
            else {
                userInfo = logIn(userInput[1], userInput[2]);
            }

            if (userInfo.empty() || userInfo[0] == "") {
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

                        // add end of session (code: 00) to the daily transaction file
                        transactionSession.addToTransFile(userInfo[0], userInfo[1], stoi(userInfo[2]), "00");
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
                /// TODO: for advertise, bid, refund; item, currentitemsfilemanager need to be fixed/implemented
                /// TODO: implement advertise.cpp
                else if (userInput[0] == "advertise") {}
                /// TODO: implement bid
                else if (userInput[0] == "bid") {}
                /// TODO: implement refund
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
}

int main() {
    runSession();

    return 0;
}
