#ifndef BID_H
#define BID_H

#include <string>
#include <vector>
#include "../user/user.h"
#include "../transactions/transaction.h"

class Bid : public Transaction {
    private:
        std::string sellerUsername;
        std::string buyerUsername;
        std::string itemKeyword;
        float bidAmount;
    public:
        Bid();
        void setSellerUsername(std::string);
        void setBuyerUsername(std::string);
        void setItemKeyword(std::string);
        void setBidAmount(float);
        std::string getSellerUsername();
        std::string getBuyerUsername();
        std::string getItemKeyword();
        float getBidAmount();
        void executeTransaction();
};

#endif
