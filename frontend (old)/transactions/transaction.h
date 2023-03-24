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

/// global variables
static double creditAddedThisSession = 0.0;
static string DAILY_TRANS_FILE;
static string CURR_USER_ACC_FILE;
static string AVAIL_ITEMS_FILE;
const string TEMP_FILE = "../files/Temp.txt";   // used as a temporary file for changing/removing values in current users file
static bool isLoggedIn = false;
const string CANCEL_COMMAND = "cancel please";

/// @brief splitIntoVector function used to split a line (string) into a vector<string>
/// @param line - line to split
/// @return - vector<string> of the line split up by spaces
vector<string> splitIntoVector(string line);

class Transaction : public User {
    public:
        /// empty constructor
        Transaction() {};

        /**
         * getters
        */
            string getName();

            string getAccountType();

            string getNameIn15Char(string name);

            double getBalance();

            string getBalanceInSpecifiedChars(double balance, int numOfCharacters);

            string getIntInSpecifiedChars(int num, int numOfCharacters);

            string getPassword();



        /**
         * setters
        */
            void setBalance(double iBalance);

            void setName(string iName);

            void setAccountType(string iAccountType);

            void setPassword(string iPassword);

            /// setFiles function to set the global static string values for the paths to the 
            /// DailyTransactions.txt and CurrentUserAccounts.txt files 
            void setFiles(string dailyTrans, string currUser, string availItems);


        
        /**
         * functions for adding/removing/updating the current users or transaction files 
        */
            /// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
            /// @param userName - username of the user who did the transaction
            /// @param accountType - account type of the user who did the transaction (ex: AA for admin)
            /// @param balance - balance of the user who did the transaction
            /// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
            void addToTransFile(string userName, string accountType, double balance, string transCode);

            /// @brief addToTransFile function used to add a transaction to the CurrentUsersFile.txt file
            /// @param userName - username of the user to add
            /// @param accountType - account type of the user to add (ex: AA for admin)
            /// @param balance - balance of the user to add
            /// @param displayPassword - displayPassword of the user (not encrypted).  This function does encrypt it here
            ///                          right before outputting to the file.
            void addToUsersFile(string userName, string accountType, double balance, string displayPassword);

            /// @brief removeFromUsersFile used to remove the information of the user
            /// from the CurrentUserAccounts.txt file
            /// @param usernameToRemove - username of the user to be removed
            void removeFromUsersFile(string usernameToRemove);

            /// @brief updateCreditInUsersFile function used to update the credit of the chosen user
            ///        in the CurrentUserAccounts.txt file
            /// @param userToUpdate - the username of the user to update the credit of
            /// @param creditToAdd - the amount of credit to add
            void updateCreditInUsersFile(string userToUpdate, double creditToAdd);

            /// @brief updatePasswordInUsersFile function used to update the password of the chosen user
            ///        in the CurrentUserAccounts.txt file
            /// @param userToUpdate - the username of the user to update the password of
            /// @param newPassword - the new password to update
            void updatePasswordInUsersFile(string userToUpdate, string newPassword);

            void addToItemsFile(string itemName, string sellerName, int daysRemaining, double minimumBid);


        /**
         * other functions which are used across the child classes such as Create, Delete, AddCredit, etc
        */
            /// @brief checkIfUserExists function used to check if the given username is a 
            /// username of an exiting user
            /// @param username - username to check exists
            /// @return - true for the user exists or false for the user does not exist
            bool checkIfExists(string thingToCheck, string filePath);

            /// @brief nameIsTooLong function used to check if the given username is too long
            /// (over 15 characters)
            /// @param username - given username to check
            /// @return - true for too long or false for a valid length
            bool nameIsTooLong(string username);
            

            /// @brief encryptPassword function used to encrypt the given password
            ///        the encrytion used is shifting over each character by +3 to the ASCII
            ///        ASCII values used are from 33 to 126.  Instead of going over 126 it goes
            ///        back to 33.
            /// @param givenPassword - given password before encryption
            /// @return - the encrypted password
            string encryptPassword(string givenPassword);

            /// @brief decryptPassword function used to decrypt the given password
            ///        shifts over the ASCII by -3 (opposite of the encryption which is +3)
            /// @param givenPassword - given password that is meant to be encrypted 
            /// @return - the decrypted password
            string decryptPassword(string givenPassword);

            /// @brief passwordAccepted function to check if the password is correct for the user given
            /// @param username - username for the user to login as
            /// @param submittedPassword - the password given to login.
            /// @return - true for the password is accepted, or false for the password is not accepted
            bool passwordAccepted(string username, string submittedPassword);

            /// @brief displayAllAccountInfo function used to nicely display all the current user info
            ///        (except for the passwords)
            /// @param accountType - string value representing the accountType for the user (e.g "AA" for admin)
            ///                      this is important because only admin users are able to use this command
            void displayAllAccountInfo(string accountType);

            /// @brief sessionCreditLimitExceeded function used to check if the limit ($1000) for 1 session is
            ///        exceeded or not
            /// @param creditToAdd - amount of credit intended to be added to the user's current balance
            /// @return - true for yes the session credit limit is exceeded, or false for no the session credit
            ///           limit is not exceeded
            bool sessionCreditLimitExceeded(double creditToAdd);

            /// @brief getBalanceFromChosenUser function used to get the balance of whatever user wanted.
            ///        This is important for when an admin is trying to add credit to another user, because
            ///        the balance of the user being added to is needed and not the admin who is doing the
            ///        addcredit transaction.
            /// @param chosenUsername - the username of the user who's balance we want to obtain
            /// @return - int for the balance that the chosen user has
            double getBalanceFromChosenUser(string chosenUsername);

            /// @brief cancelTransaction function used to determine if the user's response is to cancel the 
            ///        current transaction (if the response is the CANCEL_COMMAND (currently: "cancel please"))
            /// @param response - the string response that is to be checked
            /// @return - true for yes the user wants to cancel the transaction, or false for the user does not
            ///           want to cancel the transaction
            bool cancelTransaction(string response);

            string fillStringWithSpaces(string originalString, int numOfCharacters);

            bool is2OrLessDecimals(string response);

            void executeTransaction();
};

#endif