#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include "Record.h"
#include "User.h"
#include <string>
#include <iostream>
#include <fstream>

class FileController {
	private:
		std::string currentUsersFile;
		std::string availableItemsFile;
		std::string dailyTransactionFile;
	public:
		FileController(std::string userFile, std::string itemFile, std::string transFile);

		// User file methods
		USER_RECORD getUser(std::string lookupName);
		bool findUser(std::string lookupName);
		bool findItem(std::string lookupItem);
		ITEM_RECORD getItem(std::string lookupItem);

		void addUser(USER_RECORD newUser);
		void deleteUser(std::string username);
		void updateCredit(std::string username, float credit);
		void resetPassword(std::string username, std::string password);

		// Item file methods
		void addItem(ITEM_RECORD itemRecord);
		void displayAvailableItems();
		void displayAvailableUsers();
		float getItemBid(ITEM_RECORD itemRecord);
		void updateItemBid(ITEM_RECORD itemRecord, std::string buyer);

		// Transaction file methods
		void logTransaction(std::string transactionNumber, std::string output);
};
#endif