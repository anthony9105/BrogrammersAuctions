#include <iostream>
#include <string>
#include "User.h"
#include "CurrentUserAccountsFileManager.h"
#include "DailyTransactionFileManager.h"
#include "refund.h"

void refund(User* user) {
    std::string buyerUsername;
    std::string sellerUsername;
    std::string amount;

    std::cout << "Enter the buyer’s account name: " << std::endl;
    std::cin >> buyerUsername;
    if (buyerUsername == "exit") {
        return;
    }
    std::cout << "Enter the seller’s account name: " << std::endl;
    std::cin >> sellerUsername;
    if (sellerUsername == "exit") {
        return;
    }
    std::cout << "Enter the amount of credit to transfer: " << std::endl;
    std::cin >> amount;

    User* buyer = CurrentUserAccountsFileManager::findUser(buyerUsername);
    User* seller = CurrentUserAccountsFileManager::findUser(sellerUsername);

    if (buyer->getUsername() == "" || seller->getUsername() == "") {
        std::cout << "ERROR: One or both of the given users does not exist..." << std::endl;
        return;
    }

    DailyTransactionFileManager::addRefundTransaction(buyer->getUsername(), seller->getUsername(), amount);
}
