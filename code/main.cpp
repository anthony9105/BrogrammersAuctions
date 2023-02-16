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

        void addToFile(string userName, string accountType, int balance, string transCode) {
            // append to the file
            dailyTransFile.open(DAILY_TRANS_FILE, ios::app);

            if (dailyTransFile.is_open()) {
                dailyTransFile << transCode << " " << getNameIn15Char(userName)
                << " " << accountType << " " << getBalanceIn9Char(balance) << endl;
            }
            dailyTransFile.close();
            // transactionNames.push_back(name);
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
        void executeTransaction(string name, string accountType, int balance) {
            Transaction::addToFile(name, accountType, balance, "01");
            cout << "Create account" << endl;
        }

};

class Delete : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            Transaction::addToFile(name, accountType, balance, "02");
            cout << "Delete account" << endl;
        }
};

class Bid : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            Transaction::addToFile(name, accountType, balance, "04");
            cout << "Bid transaction" << endl;
        }
};

class AddCredit : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance) {
            Transaction::addToFile(name, accountType, balance, "06");
            cout << "Add credit transaction" << endl;
        }
};

void logIn(string username) {
    readUserAccountsFile.open(CURR_USER_ACC_FILE);
    string line;
    if (readUserAccountsFile.is_open()) {
        while ( getline(readUserAccountsFile, line)) {
            if (line.find(username) != string::npos) {
                cout << "Login successful" << endl;
                isLoggedIn = true;
            }
        }
    }

    readUserAccountsFile.close();

    if (!isLoggedIn) {
        cout << "Error. Login unsuccessful.  No account corresponding to this username" << endl;
    }

}

int main() {

    while(true) {
        string s, temp; 
        getline(cin, s);

        // split the input into a vector of strings with space as the delimiter
        stringstream ss(s);
        vector<string> userInput;
        while(getline(ss, temp, ' ')) {
            userInput.push_back(temp);
        }

        if (userInput[0] == "close") {exit(0);}
        if (!isLoggedIn) {
            if (userInput[0] == "login") {
                logIn(userInput[1]);
            }
        }
        else {
            if (userInput[0] == "logout") {}
            else if (userInput[0] == "create") {}
            else if (userInput[0] == "delete") {}
            else if (userInput[0] == "advertise") {}
            else if (userInput[0] == "bid") {}
            else if (userInput[0] == "refund") {}
            else if (userInput[0] == "addcredit") {}
        }
        // else {
        //     cout << "You are not logged in" << endl;
        // }
    }

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