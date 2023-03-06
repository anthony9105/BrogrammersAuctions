#include "bid.h"

Bid::Bid() {
    sellerUsername = "";
    buyerUsername = "";
    itemKeyword = "";
    bidAmount = 0.0;
}

void Bid::setSellerUsername(std::string sellerUsername) {
    this->sellerUsername = sellerUsername;
}

void Bid::setBuyerUsername(std::string buyerUsername) {
    this->buyerUsername = buyerUsername;
}

void Bid::setItemKeyword(std::string itemKeyword) {
    this->itemKeyword = itemKeyword;
}

void Bid::setBidAmount(float bidAmount) {
    this->bidAmount = bidAmount;
}

std::string Bid::getSellerUsername() {
    return sellerUsername;
}

std::string Bid::getBuyerUsername() {
    return buyerUsername;
}

std::string Bid::getItemKeyword() {
    return itemKeyword;
}

float Bid::getBidAmount() {
    return bidAmount;
}

void Bid::executeTransaction() {
    std::vector<std::string> currentUserInfo = getUserInfo(getName(), getAccountType(), getBalance(), getPassword());
    std::vector<std::string> sellerUserInfo = getUserInfo(sellerUsername);
    std::vector<std::string> buyerUserInfo = getUserInfo(buyerUsername);

    // checking if the seller and buyer exist
    if (sellerUserInfo.empty() || buyerUserInfo.empty()) {
        std::cout << "Error.  The buyer and/or seller does not exist." << std::endl;
        return;
    }

    // checking if the buyer is able to afford the bid
    if (std::stof(buyerUserInfo[2]) < bidAmount) {
        std::cout << "Error.  The buyer does not have enough credits for this bid." << std::endl;
        return;
    }

    // checking if the item exists and belongs to the seller
    std::ifstream readItemsFile;
    readItemsFile.open(ITEMS_FILE);
    std::string line;
    bool foundItem = false;
    while (getline(readItemsFile, line)) {
        std::vector<std::string> itemInfo = splitIntoVector(line);
        if (itemInfo[0] == itemKeyword && itemInfo[1] == sellerUsername) {
            foundItem = true;
            break;
        }
    }
    readItemsFile.close();
    if (!foundItem) {
        std::cout << "Error.  The item does not exist or does not belong to the seller." << std::endl;
        return;
    }

    // updating the balances of the buyer and seller
    float newSellerBalance = std::stof(sellerUserInfo[2]) + bidAmount;
    float newBuyerBalance = std::stof(buyerUserInfo[2]) - bidAmount;
    updateUserBalance(sellerUsername, new
