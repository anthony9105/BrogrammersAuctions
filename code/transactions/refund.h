#ifndef REFUND_H
#define REFUND_H
// NOT FIXED YET
#include "../user/user.h"
#include "../item/item.h"
#include "../filemanager/CurrentUserAccountsFileManager.h"
#include "../filemanager/CurrentItemsFileManager.h"
#include <string>

class Refund {
    public:
        static void execute(std::vector<std::string> command) {
            std::string buyer = command[1];
            std::string seller = command[2];
            float refundAmount = stof(command[3]);
            Item item = CurrentItemsFileManager::getItem(command[4]);
            User buyerObj = CurrentUserAccountsFileManager::getUser(buyer);
            User sellerObj = CurrentUserAccountsFileManager::getUser(seller);
            
            if (refundAmount > buyerObj.getBalance()) {
                std::cout << "Refund amount exceeds buyer's balance." << std::endl;
                return;
            }
            if (refundAmount > sellerObj.getBalance()) {
                std::cout << "Refund amount exceeds seller's balance." << std::endl;
                return;
            }
            
            // Update user balances and add item back to inventory
            buyerObj.setBalance(buyerObj.getBalance() - refundAmount);
            sellerObj.setBalance(sellerObj.getBalance() + refundAmount);
            item.setOwner(buyer);
            item.setSold(false);
            CurrentUserAccountsFileManager::updateUser(buyerObj);
            CurrentUserAccountsFileManager::updateUser(sellerObj);
            CurrentItemsFileManager::updateItem(item);
        }
};

#endif
