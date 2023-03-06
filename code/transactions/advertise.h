#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <string>
#include "../user/user.h"
#include "../transactions/transaction.h"

class Advertise : public Transaction {
    private:
        std::string sellerName;
        std::string itemKeyword;
        float minBid;
    public:
        Advertise();
        void executeTransaction(std::string transaction);
};

#endif
