#include "refund.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "transaction.h"
#include "../user/user.h"

Refund::Refund() {}

void Refund::executeTransaction() {
    std::string username, targetUsername, refundAmount;
    std::cout << "Please enter the username of the account to refund: ";
    std::cin >> targetUsername;
    std::cout << "Please enter the amount to refund: ";
    std::cin >> refundAmount;

    // check if the refund amount is not a valid integer
    if (!isValidNumber(refundAmount)) {
        std::cout << "Error: Invalid refund amount." << std::endl;
        return;
    }

    int refundAmountInt = stoi(refundAmount);

    // check if the refund amount is greater than the balance of the target user's account
    if (refundAmountInt > getUserBalance(targetUsername)) {
        std::cout << "Error: Refund amount is greater than target user's balance." << std::endl;
        return;
    }

    // perform refund transaction
    int balance = getUserBalance(transactionSession.getName());
    balance += refundAmountInt;
    setUserBalance(transactionSession.getName(), balance);
    balance = getUserBalance(targetUsername);
    balance -= refundAmountInt;
    setUserBalance(targetUsername, balance);

    // add refund transaction to the daily transaction file
    std::ofstream dailyTransFile;
    dailyTransFile.open(dailyTransFile_, std::ios_base::app);
    std::stringstream ss;
    ss << std::setw(15) << std::left << "05";
    ss << std::setw(15) << std::left << targetUsername;
    ss << std::setw(15) << std::left << "AA";
    ss << std::setw(15) << std::left << refundAmount;
    ss << std::endl;
    dailyTransFile << ss.str();
    dailyTransFile.close();

    std::cout << "Refund successful." << std::endl;
}
