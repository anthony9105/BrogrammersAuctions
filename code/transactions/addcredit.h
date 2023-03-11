#include <string>

#ifndef ADDCREDIT_H
#define ADDCREDIT_H

using namespace std;

/**
 * AddCredit class
*/
class AddCredit : public Transaction {
    public:
        double newBalanceOfUser;
        double executeTransaction(string name, string accountType, double balance, string userFilePath);
};

#endif