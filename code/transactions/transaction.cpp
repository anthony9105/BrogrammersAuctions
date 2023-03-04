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

string Transaction::getPassword() {
    return password;
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

void Transaction::setPassword(string iPassword) {
    while (iPassword.length() < 1 || iPassword.length() > 14) {
        cout << "Invalid.  Enter a password with more than 1 character and less than 15 characters:" << endl;
        getline(cin, iPassword);
    }

    password = iPassword;
}

void Transaction::setFiles(string dailyTrans, string currUser){
    DAILY_TRANS_FILE = dailyTrans;
    CURR_USER_ACC_FILE = currUser;
}


/// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
/// @param userName - username of the user who did the transaction
/// @param accountType - account type of the user who did the transaction (ex: AA for admin)
/// @param balance - balance of the user who did the transaction
/// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
void Transaction::addToTransFile(string userName, string accountType, int balance, string transCode) {
    ofstream dailyTransFile;

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
/// @param displayPassword - displayPassword of the user (not encrypted).  This function does encrypt it here
///                          right before outputting to the file.
void Transaction::addToUsersFile(string userName, string accountType, int balance, string displayPassword) {
    displayPassword = encryptPassword(displayPassword);

    ofstream userAccountsFile;
    // append to the file
    userAccountsFile.open(CURR_USER_ACC_FILE, ios::app);

    if (userAccountsFile.is_open()) {
        userAccountsFile << getNameIn15Char(userName)
        << " " << accountType << " " << getBalanceIn9Char(balance) << " " << displayPassword << endl;
    }
    userAccountsFile.close();
}

/// @brief removeFromUsersFile used to remove the information of the user
/// from the CurrentUserAccounts.txt file
/// @param usernameToRemove - username of the user to be removed
void Transaction::removeFromUsersFile(string usernameToRemove) {
    ifstream readUserAccountsFile;
    ofstream userAccountsFile;

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
    ifstream readUserAccountsFile;
    ofstream userAccountsFile;

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
            line = getNameIn15Char(result[0]) + " " + result[1] + " " + result[2] + " " + result[3];
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


void Transaction::updatePasswordInUsersFile(string userToUpdate, string newPassword) {
    ifstream readUserAccountsFile;
    ofstream userAccountsFile;
    
    // first open file and write everything but the line with
    // the user being deleted to a temp file
    userAccountsFile.open(TEMP_FILE);
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    while(getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        if (result[0] == userToUpdate) {
            result[3] = encryptPassword(newPassword);
            line = getNameIn15Char(result[0]) + " " + result[1] + " " + result[2] + " " + result[3];
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
    ifstream readUserAccountsFile;
    readUserAccountsFile.open(CURR_USER_ACC_FILE);

    string line;
    vector<string> result;
    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            // if username is found
            if (result[i] == username) {
                readUserAccountsFile.close();
                return true;
            }
        }
    }

    return false;
}

int Transaction::getBalanceFromChosenUser(string chosenUsername) {
     ifstream readUserAccountsFile;
    readUserAccountsFile.open(CURR_USER_ACC_FILE);

    string line;
    vector<string> result;
    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            // if username is found
            if (result[i] == chosenUsername) {
                readUserAccountsFile.close();
                return stoi(result[i+2]);
            }
        }
    }

    return 0;
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

string Transaction::encryptPassword(string givenPassword) {
    string encryptedPassword= "";

    for (int i=0; i < givenPassword.length(); i++) {
        char tempChar = givenPassword.at(i);
        int tempInt = int(tempChar);
        
        tempInt += 3;

        // make sure the characters are only from ! to ~ (33 to 126)
        if (tempInt > 126) {
            tempInt = tempInt - (127-33);
        }

        encryptedPassword += char(tempInt);
    }

    return encryptedPassword;
}

string Transaction::decryptPassword(string givenPassword) {
    string decryptedPassword = "";

        for (int i=0; i < givenPassword.length(); i++) {
        char tempChar = givenPassword.at(i);
        int tempInt = int(tempChar);
        
        tempInt -= 3;

        // make sure the characters are only from ! to ~ (33 to 126)
        if (tempInt < 33) {
            tempInt = tempInt + (127-33);
        }

        decryptedPassword += char(tempInt);
    }

    return decryptedPassword;
}

bool Transaction::passwordAccepted(string username, string submittedPassword) {
    ifstream readUserAccountsFile;

    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    vector<string> result;
    string encryptedPassword = encryptPassword(submittedPassword);

    while (getline(readUserAccountsFile, line)) {
        result = splitIntoVector(line);

        for (int i=0; i < result.size(); i++) {
            // if password is correct
            if (result[i] == encryptedPassword && result[0] == username) {
                //cout << "Login successful" << endl;
                //isLoggedIn = true;
                readUserAccountsFile.close();
                return true;
            }
        }
    }

    return false;
}

void Transaction::displayAllAccountInfo(string accountType) {
    if (accountType == "AA") {
        cout.width(20);
        cout << "Username";
        cout.width(20);
        cout << "Account Type";
        cout.width(20);
        cout << "Credit" << endl;

        ifstream readUserAccountsFile;

        readUserAccountsFile.open(CURR_USER_ACC_FILE);
        string line;
        vector<string> result;
        while (getline(readUserAccountsFile, line)) {
            result = splitIntoVector(line);

            for (int i=0; i < result.size(); i++) {
                // dont display the password (last element in result)
                if (i < result.size()-1) {
                    cout.width(20);
                    cout << result[i];
                }
            }
            cout << endl;
        }
    }
    else {
        cout << "You do not have the privilege to do this" << endl;
    }
}

bool Transaction::sessionCreditLimitExceeded(int creditToAdd) {
    if (creditAddedThisSession + creditToAdd > 1000) {
        return true;
    }

    creditAddedThisSession += creditToAdd;
    return false;
}

void Transaction::executeTransaction() {}