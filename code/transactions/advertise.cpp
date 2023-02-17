#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "../user/user.h"
#include "transaction.h"
#include "advertise.h"

void AdvertiseTransaction::execute() {
  const int nameLimit = 25;
  const double bidLimit = 999.99;
  const int maxDays = 100;

  std::string itemName;
  std::string minimumBid;
  int numDays;

  std::cout << "Enter Item Name: ";
  std::cin.ignore();
  std::getline(std::cin, itemName);

  if (itemName.length() <= nameLimit) {
    std::cout << "Enter starting bid: ";
    std::cin >> minimumBid;
    if (std::stod(minimumBid) <= bidLimit) {
      std::cout << "Enter auction end date: ";
      std::cin >> numDays;
      if (numDays <= maxDays) {
        std::cout << itemName << " has now been posted." << std::endl;
        DailyTransactionFileManager::addAdvertiseTransaction(itemName, CurrentUserAccountsFileManager::getCurrentUsername(), numDays, minimumBid);
      } else {
        std::cout << "Error: Number of days exceeds max days." << std::endl;
        return;
      }
    } else {
      std::cout << "Error: Bid price exceeds max limit." << std::endl;
      return;
    }
  } else {
    std::cout << "Error: Item name exceeds character limit." << std::endl;
    return;
  }
}
