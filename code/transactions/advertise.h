#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <iostream>
#include <string>
#include "../user/user.h"
#include "../transactions/item.h"
#include "../transactions/transaction.h"
#include "../transactions/currentitemsfilemanager.h"

class Advertise : public Transaction {
public:
    // Constructor
    Advertise(User* user, CurrentItemsFileManager* itemManager, CurrentTransactionItemsFileManager* transactionItemsManager);


    // Execute the advertise transaction
    bool execute();
};

#endif
