#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <string>
#include "../user/user.h"
#include "../transactions/transaction.h"

using namespace std;

class Advertise : public Transaction {
    public:
        void executeTransaction(string name, string accountType, double balance, string availItemsFile);
};

#endif