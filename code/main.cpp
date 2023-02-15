#include <iostream>
#include <vector>
using namespace std;

class User {
    public:
        int balance = 0;
        string name;
        string accountType;

        // constructor
        User(string iName, string iAccountType) {
            name = iName;
            accountType = iAccountType;
        }
};

class Transaction {
    public:
        vector<string> transactionNames;
        // string name;

        // // constructor
        // Transaction(string iName) {
        //     name = iName;
        // }
        void addToList(string name) {
            transactionNames.push_back(name);
        }

        void printAllTransactions() {
            for (int i=0; i < transactionNames.size(); i++){
                cout << transactionNames[i] << endl;
            }
        }
        void executeTransaction() {}
};

class Create : public Transaction {
    public:
        void executeTransaction() {
            Transaction::addToList("Create");
            cout << "Create account" << endl;
        }

};

class Delete : public Transaction {
    public:
        void executeTransaction() {
            Transaction::addToList("Delete");
            cout << "Delete account" << endl;
        }
};

class Bid : public Transaction {
    public:
        void executeTransaction() {
            Transaction::addToList("Bid");
            cout << "Bid transaction" << endl;
        }
};

class AddCredit : public Transaction {
    public:
        void executeTransaction() {
            Transaction::addToList("Add Credit");
            cout << "Add credit transaction" << endl;
        }
};

int main() {
    User user1("Anthony", "AA");
    Transaction transactionSession;
    Create create1;
    create1.executeTransaction();
    Bid bid1;
    bid1.executeTransaction();
    transactionSession.printAllTransactions();

    return 0;
}