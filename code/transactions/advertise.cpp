/*#include "advertise.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "transaction.h"
#include "../user/user.h"

Advertise::Advertise(std::string dailyTransFileName) {
    sellerName = "";
    itemKeyword = "";
    minBid = 0.0f;
    this->dailyTransFileName = dailyTransFileName;
}

void Advertise::executeTransaction(std::string transaction) {
    std::stringstream stream(transaction);
    std::string word;

    std::vector<std::string> words;
    while (stream >> word) {
        words.push_back(word);
    }

    if (words.size() != 4) {
        std::cout << "ERROR: Invalid advertise transaction format" << std::endl;
        return;
    }

    // check if seller is logged in
    if (getName() != words[1]) {
        std::cout << "ERROR: Seller is not logged in" << std::endl;
        return;
    }

    // get seller's balance
    int balance = getBalance();

    // check if balance is negative
    if (balance < 0) {
        std::cout << "ERROR: Seller's balance is negative" << std::endl;
        return;
    }

    // check if balance is less than 1
    if (balance < 1) {
        std::cout << "ERROR: Seller's balance is less than 1" << std::endl;
        return;
    }

    // check if item keyword is greater than 15 characters
    if (words[2].length() > 15) {
        std::cout << "ERROR: Item keyword length exceeds 15 characters" << std::endl;
        return;
    }

    // check if min bid is greater than or equal to 0.01
    float minBid = std::stof(words[3]);
    if (minBid < 0.01) {
        std::cout << "ERROR: Minimum bid must be greater than or equal to 0.01" << std::endl;
        return;
    }

    // use functions in the parent class Transaction
    Transaction::addToUsersFile(sellerName, itemKeyword, balance, "05");
    cout << "Advertise successful" << endl;
    return;
}
*/