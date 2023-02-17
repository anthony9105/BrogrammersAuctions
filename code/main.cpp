#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const string DAILY_TRANS_FILE = "../files/DailyTransactions.txt";
const string CURR_USER_ACC_FILE = "../files/CurrentUserAccounts.txt";
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
            string temp = "";
            temp += to_string(balance);
            while (temp.length() < 9) {
                temp += "0";
            }

            return temp;
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
            Transaction::addToTransFile(name, accountType, balance, "02");
            cout << "Delete account" << endl;
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
            Transaction::addToTransFile(name, accountType, balance, "06");
            cout << "Add credit transaction" << endl;
        }
};

// vector<string> splitIntoVector(string line) {
//     string temp;

//     // split the input into a vector of strings with space as the delimiter
//     stringstream ss(line);
//     vector<string> finalVector;
//     while(getline(ss, temp, ' ')) {
//         if (temp != "  ")
//             finalVector.push_back(temp);
//     }

//     return finalVector;
// }

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
                else if (userInput[0] == "delete") {}
                else if (userInput[0] == "advertise") {}
                else if (userInput[0] == "bid") {}
                else if (userInput[0] == "refund") {}
                else if (userInput[0] == "addcredit") {}
            }
        }
    } while (userInput.empty() || userInput[0] != "close");

    // Transaction transactionSession;
    // transactionSession.setName("Anthony");
    // transactionSession.setAccountType("AA");
    // cout << transactionSession.getName() << " " << transactionSession.getAccountType() << endl;
    // Create create1;
    // create1.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
    // Bid bid1;
    // bid1.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
    // bid1.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
    // create1.executeTransaction(transactionSession.getName(), transactionSession.getAccountType(), transactionSession.getBalance());
    // transactionSession.printAllTransactions();

    return 0;
}