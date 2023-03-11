#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <iomanip>

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

    double Transaction::getBalance() {
        return balance;
    }

    string Transaction::getBalanceInSpecifiedChars(double balance, int numOfCharacters) {
        string temp1 = "";
        string temp2 = "";
        stringstream ss;
        ss << fixed << setprecision(2) << balance;
        temp1 = ss.str();

        while (temp2.length() < numOfCharacters - temp1.length()) {
            temp2 += "0";
        }
        temp2 += temp1;
        return temp2;
    }

    string Transaction::getIntInSpecifiedChars(int num, int numOfCharacters) {
        string temp1 = "";
        string temp2 = "";
        stringstream ss;
        temp1 += to_string(num);

        while (temp2.length() < numOfCharacters - temp1.length()) {
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
    void Transaction::setBalance(double iBalance) {
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

    void Transaction::setFiles(string dailyTrans, string currUser, string availIems){
        DAILY_TRANS_FILE = dailyTrans;
        CURR_USER_ACC_FILE = currUser;
        AVAIL_ITEMS_FILE = availIems;
    }


/**
 * functions for adding/removing/updating the current users or transaction files 
*/
    /// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
    /// @param userName - username of the user who did the transaction
    /// @param accountType - account type of the user who did the transaction (ex: AA for admin)
    /// @param balance - balance of the user who did the transaction
    /// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
    void Transaction::addToTransFile(string userName, string accountType, double balance, string transCode) {
        ofstream dailyTransFile;

        // append to the file
        dailyTransFile.open(DAILY_TRANS_FILE, ios::app);

        // write new line to the file
        if (dailyTransFile.is_open()) {
            dailyTransFile << transCode << " " << getNameIn15Char(userName)
            << " " << accountType << " " << getBalanceInSpecifiedChars(balance, 12) << endl;
        }

        dailyTransFile.close();
    }

    /// @brief addToUsersFile function used to add a transaction to the CurrentUsersFile.txt file
    /// @param userName - username of the user to add
    /// @param accountType - account type of the user to add (ex: AA for admin)
    /// @param balance - balance of the user to add
    /// @param displayPassword - displayPassword of the user (not encrypted).  This function does encrypt it here
    ///                          right before outputting to the file.
    void Transaction::addToUsersFile(string userName, string accountType, double balance, string displayPassword) {
        displayPassword = encryptPassword(displayPassword);

        ofstream userAccountsFile;
        // append to the file
        userAccountsFile.open(CURR_USER_ACC_FILE, ios::app);

        if (userAccountsFile.is_open()) {
            userAccountsFile << getNameIn15Char(userName)
            << " " << accountType << " " << getBalanceInSpecifiedChars(balance, 12) << " " << displayPassword << endl;
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
    void Transaction::updateCreditInUsersFile(string userToUpdate, double creditToAdd) {
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
                double newBalance = stod(result[2]) + creditToAdd;
                result[2] = getBalanceInSpecifiedChars(newBalance, 12);
                //result[2] = getBalanceInSpecifiedChars((stod(result[2])) + creditToAdd, 12);
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

    void Transaction::addToItemsFile(string itemName, string sellerName, int daysRemaining, double minimumBid) {
        ofstream itemsFile;
        // append to the file
        itemsFile.open(AVAIL_ITEMS_FILE, ios::app);

        if (itemsFile.is_open()) {
            itemsFile << fillStringWithSpaces(itemName, 25) << " " << fillStringWithSpaces(sellerName, 15)
            << " " << fillStringWithSpaces(" ", 15) << " " << getIntInSpecifiedChars(daysRemaining, 3) << " "  << getBalanceInSpecifiedChars(minimumBid, 9) << endl;
        }
        itemsFile.close();
    }
/**
 ***************************************************************************
*/



/**
 * other functions which are used across the child classes such as Create, Delete, AddCredit, etc
*/
    /// @brief checkIfExists function used to check if the given username/item/etc is a 
    /// username of an exiting user or existing item, etc
    /// @param thingToCheck - thing to check if exists
    /// @return - true for the thing exists or false for the thing does not exist
    bool Transaction::checkIfExists(string thingToCheck, string filePath) {
        ifstream readFile;
        readFile.open(filePath);

        string line;
        vector<string> result;
        while (getline(readFile, line)) {
            result = splitIntoVector(line);

            for (int i=0; i < result.size(); i++) {
                // if thingToCheck is found
                if (result[i] == thingToCheck) {
                    readFile.close();
                    return true;
                }
            }
        }

        return false;
    }

    double Transaction::getBalanceFromChosenUser(string chosenUsername) {
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
                    return stod(result[i+2]);
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

    bool Transaction::sessionCreditLimitExceeded(double creditToAdd) {
        if (creditAddedThisSession + creditToAdd > 1000.00) {
            return true;
        }

        creditAddedThisSession += creditToAdd;
        return false;
    }

    bool Transaction::cancelTransaction(string response) {
        if (response == CANCEL_COMMAND) {
            cout << "Transaction cancelled" << endl;
            return true;
        }
        return false;
    }

    string Transaction::fillStringWithSpaces(string originalString, int numOfCharacters) {
        string newString;
        newString = originalString;
        while(newString.length() < numOfCharacters) {
            newString += " ";
        }

        return newString;
    }

    bool Transaction::is2OrLessDecimals(string response) {
        // get the number of decimal places
        int numOfDecimals = -1;
        bool decimalFound = false;

        for (int i=0; i < response.length() ; i++) {
            if (response[i] == '.') {
                decimalFound = true;
            }

            if (decimalFound) {
                numOfDecimals++;
            }
        }
        // if the decimal places is only 2 or less
        if (numOfDecimals <= 2) {
            return true;
        }
        return false;
    }

    void Transaction::executeTransaction() {}

/**
 ***********************************************************
*/