#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include "../user/user.h"
#include "transaction.h"

using namespace std;

/// @brief splitIntoVector function used to split a line (string) into a vector<string>
/// @param line - line to split
/// @return - vector<string> of the line split up by spaces
vector<string> splitIntoVector(string line) {
    stringstream ss(line);
    string word;
    vector<string> result;

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

/**
 * getters
*/
string Transaction::getName() {
    return name;
}

string Transaction::getAccountType() {
    return accountType;
}

string Transaction::getNameIn15Char(string name) {
    string temp = "";
    temp += name;
    while (temp.length() < 15) {
        temp += " ";
    }

    return temp;
}

int Transaction::getBalance() {
    return balance;
}

string Transaction::getBalanceIn9Char(int balance) {
    string temp1 = "";
    string temp2 = "";
    temp1 += to_string(balance);

    while (temp2.length() < 9 - temp1.length()) {
        temp2 += "0";
    }
    temp2 += temp1;
    return temp2;
}


/**
 * setters
*/
void Transaction::setBalance(int iBalance) {
    balance = iBalance;
}

void Transaction::setName(string iName) {
    name = iName;
}

void Transaction::setAccountType(string iAccountType) {
    accountType = iAccountType;
}


/// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
/// @param userName - username of the user who did the transaction
/// @param accountType - account type of the user who did the transaction (ex: AA for admin)
/// @param balance - balance of the user who did the transaction
/// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
void Transaction::addToTransFile(string userName, string accountType, int balance, string transCode) {
    // append to the file
    dailyTransFile.open(DAILY_TRANS_FILE, ios::app);

    // write new line to the file
    if (dailyTransFile.is_open()) {
        dailyTransFile << transCode << " " << getNameIn15Char(userName)
        << " " << accountType << " " << getBalanceIn9Char(balance) << endl;
    }

    dailyTransFile.close();
}

/// @brief addToUsersFile function used to add a transaction to the CurrentUsersFile.txt file
/// @param userName - username of the user to add
/// @param accountType - account type of the user to add (ex: AA for admin)
/// @param balance - balance of the user to add
void Transaction::addToUsersFile(string userName, string accountType, int balance) {
    
    // append to the file
    userAccountsFile.open(CURR_USER_ACC_FILE, ios::app);
    cout << getNameIn15Char(userName)
        << " " << accountType << " " << getBalanceIn9Char(balance) << endl;

    if (userAccountsFile.is_open()) {
        userAccountsFile << getNameIn15Char(userName)
        << " " << accountType << " " << getBalanceIn9Char(balance) << endl;
    }
    userAccountsFile.close();
}

/// @brief removeFromUsersFile used to remove the information of the user
/// from the CurrentUserAccounts.txt file
/// @param usernameToRemove - username of the user to be removed
void Transaction::removeFromUsersFile(string usernameToRemove) {
    // first open file and write everything but the line with
    // the user being deleted to a temp file
    userAccountsFile.open(TEMP_FILE);
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while(getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        if (result[0] != usernameToRemove) {
            userAccountsFile << line << endl;
        }
    }
    userAccountsFile.close();
    readUserAccountsFile.close();

    // next read from the temp file and print everything from there
    // to the currentuseraccounts.txt file
    readUserAccountsFile.open(TEMP_FILE);
    userAccountsFile.open(CURR_USER_ACC_FILE);
    while(getline(readUserAccountsFile, line)) {
        userAccountsFile << line << endl;
    }

    readUserAccountsFile.close();
    userAccountsFile.close();

    // finally delete the temp file since it is no longer needed
    remove(TEMP_FILE.c_str());
}

/// @brief updateCreditInUsersFile function used to update the credit of the chosen user
/// @param userToUpdate - the username of the user to update the credit of
/// @param creditToAdd - the amount of credit to add
void Transaction::updateCreditInUsersFile(string userToUpdate, int creditToAdd) {
    // first open file and write everything but the line with
    // the user being deleted to a temp file
    userAccountsFile.open(TEMP_FILE);
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while(getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        if (result[0] == userToUpdate) {
            cout << "before: " << result[2] << endl;
            result[2] = getBalanceIn9Char((stoi(result[2])) + creditToAdd);
            cout << "after: " << result[2] << endl;
            line = getNameIn15Char(result[0]) + " " + result[1] + " " + result[2];
        }
        userAccountsFile << line << endl;
    }
    userAccountsFile.close();
    readUserAccountsFile.close();

    // next read from the temp file and print everything from there
    // to the currentuseraccounts.txt file
    readUserAccountsFile.open(TEMP_FILE);
    userAccountsFile.open(CURR_USER_ACC_FILE);
    while(getline(readUserAccountsFile, line)) {
        userAccountsFile << line << endl;
    }

    readUserAccountsFile.close();
    userAccountsFile.close();

    // finally delete the temp file since it is no longer needed
    remove(TEMP_FILE.c_str());
}

/// @brief checkIfUserExists function used to check if the given username is a 
/// username of an exiting user
/// @param username - username to check exists
/// @return - true for the user exists or false for the user does not exist
bool Transaction::checkIfUserExists(string username) {
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            // if username is found
            if (result[i] == username) {
                cout << "Login successful" << endl;
                isLoggedIn = true;
                readUserAccountsFile.close();
                return true;
            }
        }
    }

    return false;
}

/// @brief nameIsTooLong function used to check if the given username is too long
/// (over 15 characters)
/// @param username - given username to check
/// @return - true for too long or false for a valid length
bool Transaction::nameIsTooLong(string username) {
    if (username.length() > 15) {
        return true;
    }
    return false;
}

void Transaction::executeTransaction() {}