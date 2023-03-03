#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

#ifndef TRANSACTION_H
#define TRANSACTION_H

using namespace std;

const string DAILY_TRANS_FILE = "../files/DailyTransactions.txt";
const string CURR_USER_ACC_FILE = "../files/CurrentUserAccounts.txt";
const string TEMP_FILE = "../files/Temp.txt";
static bool isLoggedIn = false;
static ofstream dailyTransFile;
static ifstream readUserAccountsFile;
static ofstream userAccountsFile;

/// @brief splitIntoVector function used to split a line (string) into a vector<string>
/// @param line - line to split
/// @return - vector<string> of the line split up by spaces
vector<string> splitIntoVector(string line);

class Transaction : public User {
    public:
        vector<string> transactionNames;

        Transaction() {};

        /**
         * getters
        */
        string getName();

        string getAccountType();

        string getNameIn15Char(string name);

        int getBalance();

        string getBalanceIn9Char(int balance);

        string getPassword();


        /**
         * setters
        */
        void setBalance(int iBalance);

        void setName(string iName);

        void setAccountType(string iAccountType);

        void setPassword(string iPassword);


        /// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
        /// @param userName - username of the user who did the transaction
        /// @param accountType - account type of the user who did the transaction (ex: AA for admin)
        /// @param balance - balance of the user who did the transaction
        /// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
        void addToTransFile(string userName, string accountType, int balance, string transCode);

        /// @brief addToTransFile function used to add a transaction to the CurrentUsersFile.txt file
        /// @param userName - username of the user to add
        /// @param accountType - account type of the user to add (ex: AA for admin)
        /// @param balance - balance of the user to add
        /// @param displayPassword - displayPassword of the user (not encrypted).  This function does encrypt it here
        ///                          right before outputting to the file.
        void addToUsersFile(string userName, string accountType, int balance, string displayPassword);

        /// @brief removeFromUsersFile used to remove the information of the user
        /// from the CurrentUserAccounts.txt file
        /// @param usernameToRemove - username of the user to be removed
        void removeFromUsersFile(string usernameToRemove);

        /// @brief updateCreditInUsersFile function used to update the credit of the chosen user
        /// @param userToUpdate - the username of the user to update the credit of
        /// @param creditToAdd - the amount of credit to add
        void updateCreditInUsersFile(string userToUpdate, int creditToAdd);

        void updatePasswordInUsersFile(string userToUpdate, string newPassword);

        /// @brief checkIfUserExists function used to check if the given username is a 
        /// username of an exiting user
        /// @param username - username to check exists
        /// @return - true for the user exists or false for the user does not exist
        bool checkIfUserExists(string username);

        /// @brief nameIsTooLong function used to check if the given username is too long
        /// (over 15 characters)
        /// @param username - given username to check
        /// @return - true for too long or false for a valid length
        bool nameIsTooLong(string username);
        
        string encryptPassword(string givenPassword);

        string decryptPassword(string givenPassword);

        bool passwordAccepted(string username, string submittedPassword);


        void executeTransaction();
};

#endif