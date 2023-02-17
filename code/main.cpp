#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
using namespace std;

const string DAILY_TRANS_FILE = "../files/DailyTransactions.txt";
const string CURR_USER_ACC_FILE = "../files/CurrentUserAccounts.txt";
const string TEMP_FILE = "../files/Temp.txt";
bool isLoggedIn = false;
ofstream dailyTransFile;
ifstream readUserAccountsFile;
ofstream userAccountsFile;

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
 * User class
*/
class User {
    protected:
        int balance = 0;
        string name;
        string accountType;

    public:
        // constructor
        User(string iName, string iAccountType) {
            name = iName;
            accountType = iAccountType;
        }

        User() {}
};

/**
 * Transaction session class
*/
class Transaction : public User {
    public:
        vector<string> transactionNames;

        /**
         * getters
        */
        string getName() {
            return name;
        }

        string getAccountType() {
            return accountType;
        }

        string getNameIn15Char(string name) {
            string temp = "";
            temp += name;
            while (temp.length() < 15) {
                temp += " ";
            }

            return temp;
        }

        int getBalance() {
            return balance;
        }

        string getBalanceIn9Char(int balance) {
            string temp1 = "", temp2 = "";
            temp1 += to_string(balance);
            while (temp1.length() - temp2.length() + 2 < 9) {
                temp2 += "0";
            }
            temp2 += temp1;
            return temp2;
        }


        /**
         * setters
        */
        void setBalance(int iBalance) {
            balance = iBalance;
        }

        void setName(string iName) {
            name = iName;
        }

        void setAccountType(string iAccountType) {
            accountType = iAccountType;
        }


        /// @brief addToTransFile function used to add a transaction to the DailyTransactions.txt file
        /// @param userName - username of the user who did the transaction
        /// @param accountType - account type of the user who did the transaction (ex: AA for admin)
        /// @param balance - balance of the user who did the transaction
        /// @param transCode - transaction code which represents the type of transaction (ex: 01 for create)
        void addToTransFile(string userName, string accountType, int balance, string transCode) {
            // append to the file
            dailyTransFile.open(DAILY_TRANS_FILE, ios::app);

            // write new line to the file
            if (dailyTransFile.is_open()) {
                dailyTransFile << transCode << " " << getNameIn15Char(userName)
                << " " << accountType << " " << getBalanceIn9Char(balance) << endl;
            }

            dailyTransFile.close();
        }

        /// @brief addToTransFile function used to add a transaction to the CurrentUsersFile.txt file
        /// @param userName - username of the user to add
        /// @param accountType - account type of the user to add (ex: AA for admin)
        /// @param balance - balance of the user to add
        void addToUsersFile(string userName, string accountType, int balance) {
            
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
        void removeFromUsersFile(string usernameToRemove) {
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
        void updateCreditInUsersFile(string userToUpdate, int creditToAdd) {
            // first open file and write everything but the line with
            // the user being deleted to a temp file
            userAccountsFile.open(TEMP_FILE);
            readUserAccountsFile.open(CURR_USER_ACC_FILE);
            string line;
            vector<string> result;
            while(getline(readUserAccountsFile, line)) {
                result = splitIntoVector(line);

                if (result[0] == userToUpdate) {
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
        bool checkIfUserExists(string username) {
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
        bool nameIsTooLong(string username) {
            if (username.length() > 15) {
                return true;
            }
            return false;
        }

        void printAllTransactions() {
            for (int i=0; i < transactionNames.size(); i++){
                cout << transactionNames[i] << endl;
            }
        }

        bool hasGeneralPermission(string accountType, string buyOrSell) {
            if (accountType == "AA" || accountType == "FS") {
                return true;
            }
            else if (accountType == "BS" && buyOrSell == "buy") {
                return true;
            }
            else if (accountType == "SS" && buyOrSell == "sell") {
                return true;
            }
            else {
                return false;
            }
        }

        void executeTransaction() {}
};


/**
 * Create class
*/
class Create : public Transaction {
    public:
        void executeTransaction() {
            bool usernameIsInUse = true;
            bool nameTooLong = true;
            bool validAccountType = false;
            string username, accountType;
            
            cout << "Enter new username" << endl;

            // while loop that does not end until valid input is entered
            while (usernameIsInUse || nameTooLong) {
                getline(cin, username);
                usernameIsInUse = Transaction::checkIfUserExists(username);
                nameTooLong = Transaction::nameIsTooLong(username);

                if (usernameIsInUse) {
                    cout << "Error.  Username is already in use.  Enter a new username" << endl;
                }

                if (nameTooLong) {
                    cout << "Error.  Username is too long.  Enter a new username" << endl;
                }
            }

            cout << "Enter type of user (admin (AA), full-standard (FS), buy-standard (BS), or sell-standard (SS))" << endl;
            // while loop that does not end until valid input is entered
            while (!validAccountType) {
                getline(cin, accountType);
                if (accountType == "AA" || accountType == "FS" || accountType == "BS" || accountType == "SS") {
                    validAccountType = true;
                }
                cout << "Please enter a valid type of user (admin (AA), full-standard (FS), buy-standard (BS), or sell-standard (SS))" << endl;
            }

            // use functions in the parent class Transaction
            Transaction::addToUsersFile(username, accountType, balance);
            Transaction::addToTransFile(username, accountType, balance, "01");

            cout << "Success.  New user added." << endl;
        }

};

/**
 * Delete class
*/
class Delete : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            string nameToDelete;
            bool usernameExists = false;

            cout << "Enter username of user to delete" << endl;

            // while loop that does not end until valid input is entered
            while (!usernameExists) {
                getline(cin, nameToDelete);
                usernameExists = Transaction::checkIfUserExists(nameToDelete);

                if (!usernameExists) {
                    cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
                }
            }

            // use functions in the parent class Transaction
            Transaction::removeFromUsersFile(nameToDelete);
            Transaction::addToTransFile(name, accountType, balance, "02");
            cout << "User account deleted" << endl;
        }
};

/**
 * incomplete
*/
class Bid : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            Transaction::addToTransFile(name, accountType, balance, "04");
            cout << "Bid transaction" << endl;
        }
};

class AddCredit : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            string nameToAddCredit;
            nameToAddCredit = name;
            bool usernameExists = false;

            if (accountType == "AA") {
                cout << "Enter username of account to add credit to" << endl;

                // while loop that does not end until valid input is entered        
                while (!usernameExists) {
                    getline(cin, nameToAddCredit);
                    usernameExists = Transaction::checkIfUserExists(nameToAddCredit);

                    if (!usernameExists) {
                        cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
                    }
                }
            }

            int creditToAdd;
            cout << "Enter credit to add to user" << endl;
            cin >> creditToAdd;

            // use functions in the parent class Transaction
            Transaction::updateCreditInUsersFile(nameToAddCredit, creditToAdd);
            Transaction::addToTransFile(name, accountType, balance, "06");
            cout << "Add credit successful" << endl;

            // seg fault after here for some reason
        }
};


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
                else if (userInput[0] == "advertise") {}
                else if (userInput[0] == "bid") {}
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