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

vector<string> splitIntoVector(string line) {
    stringstream ss(line);
    string word;
    vector<string> result;

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

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

class Transaction : public User {
    public:
        vector<string> transactionNames;

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

        void setBalance(int iBalance) {
            balance = iBalance;
        }

        void setName(string iName) {
            name = iName;
        }

        void setAccountType(string iAccountType) {
            accountType = iAccountType;
        }

        void addToTransFile(string userName, string accountType, int balance, string transCode) {
            // append to the file
            dailyTransFile.open(DAILY_TRANS_FILE, ios::app);

            if (dailyTransFile.is_open()) {
                dailyTransFile << transCode << " " << getNameIn15Char(userName)
                << " " << accountType << " " << getBalanceIn9Char(balance) << endl;
            }
            dailyTransFile.close();
            // transactionNames.push_back(name);
        }

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

        bool checkIfUserExists(string username) {
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
                        return true;
                    }
                }
            }

            return false;
        }

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

class Create : public Transaction {
    public:
        void executeTransaction() {
            bool usernameIsInUse = true;
            bool nameTooLong = true;
            bool validAccountType = false;
            string username, accountType;
            
            cout << "Enter new username" << endl;
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
            getline(cin, accountType);
            while (!validAccountType) {
                if (accountType == "AA" || accountType == "FS" || accountType == "BS" || accountType == "SS") {
                    validAccountType = true;
                }
            }

            Transaction::addToUsersFile(username, accountType, balance);
            Transaction::addToTransFile(username, accountType, balance, "01");
            cout << "Success.  New user added." << endl;
        }

};

class Delete : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            string nameToDelete;
            bool usernameExists = false;

            cout << "Enter username of user to delete" << endl;
            while (!usernameExists) {
                getline(cin, nameToDelete);
                usernameExists = Transaction::checkIfUserExists(nameToDelete);

                if (!usernameExists) {
                    cout << "Error.  Username does not exist.  Enter a username of an existing user" << endl;
                }
            }

            Transaction::removeFromUsersFile(nameToDelete);
            Transaction::addToTransFile(name, accountType, balance, "02");
            cout << "User account deleted" << endl;
        }
};

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
            Transaction::updateCreditInUsersFile(nameToAddCredit, creditToAdd);
            //Transaction::setBalance(Transaction::getBalance() + creditToAdd);
            Transaction::addToTransFile(name, accountType, balance, "06");
            cout << "Add credit successful" << endl;

            // seg fault after here for some reason
        }
};

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
                transactionSession.setName(userInfo[0]);
                transactionSession.setAccountType(userInfo[1]);
                transactionSession.setBalance(stoi(userInfo[2]));
            }
        }
        else if (userInput[0] == "create") {
            Create createTransaction;
            createTransaction.executeTransaction();
        }
        else {
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
                else if (userInput[0] == "addcredit") {
                    AddCredit addCreditTransaction;
                    addCreditTransaction.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
                }
                else {
                    cout << "Unknown command.  Try again" << endl;
                }
            }
        }
    } while (userInput.empty() || userInput[0] != "close");

    return 0;
}