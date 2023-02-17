#include <string>

#ifndef ADDCREDIT_H
#define ADDCREDIT_H

using namespace std;

/**
 * AddCredit class
*/
class AddCredit : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance);
};

#endif