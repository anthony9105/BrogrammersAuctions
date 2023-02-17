#include "Advertise.h"

#include <iostream>
#include <string>

Advertise::Advertise(User* user, CurrentItemsFileManager* itemManager, TransactionItemsFileManager* transactionItemsFileManager)
    : Transaction(user, itemManager), transactionItemsFileManager(transactionItemsFileManager)
{
}

bool Advertise::execute()
{
    // Check if user balance is sufficient for advertising
    double userBalance = getUser()->getBalance();
    if (userBalance <= 0) {
        std::cout << "Transaction failed. User balance is not sufficient for advertisement." << std::endl;
        return false;
    }

    // Get the item information from the user
    std::string itemName;
    double itemPrice;
    int itemQuantity;
    std::cout << "Enter item name: ";
    std::getline(std::cin, itemName);
    std::cout << "Enter item price: ";
    std::cin >> itemPrice;
    std::cout << "Enter item quantity: ";
    std::cin >> itemQuantity;

    // Check if the item name is already in use
    if (getItemManager()->itemExists(itemName)) {
        std::cout << "Transaction failed. Item name already exists." << std::endl;
        return false;
    }

    // Create a new item and add it to the item manager
    Item newItem(itemName, itemPrice, itemQuantity);
    getItemManager()->addItem(newItem);

    // Deduct the user's balance by the advertisement fee
    double fee = 0.1 * itemPrice;
    getUser()->deductBalance(fee);

    // Add transaction to the transaction file
    TransactionItem transactionItem;
    transactionItem.setTransactionType(TransactionType::Advertise);
    transactionItem.setSellerUsername(getUser()->getUsername());
    transactionItem.setItemName(itemName);
    transactionItem.setItemPrice(itemPrice);
    transactionItem.setItemQuantity(itemQuantity);
    transactionItemsFileManager->addTransactionItem(transactionItem);

    std::cout << "Transaction successful. Item " << itemName << " advertised for sale." << std::endl;
    return true;
}
