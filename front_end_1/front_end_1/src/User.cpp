#include "User.h"
#include "AuctionSystem.h"
#include <string>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <cmath>

// Returns current user information as a string
std::string User::toString(){
	std::stringstream userStream;
	userStream << username << " " << accountType << " " << credit << " " << password;
	std::string userInfo = userStream.str();
	return userInfo;
}

// creates a user account, and checks thersholds
USER_RECORD User::CreateAccount() {
	USER_RECORD newUser;		// Initialize new USER_RECORD for newly created user

	std::printf("Enter a user type:\n");
	std::cin >> newUser.accountType;
	std::cin.ignore();

	// if input is "cancel"
	if (cancelCommandEntered(newUser.accountType)) {
		newUser.accountType = "ER";
		return newUser;
	}

	if (newUser.accountType != "AA" && newUser.accountType != "FS" && newUser.accountType != "BS" && newUser.accountType != "SS" && newUser.accountType != "AM") {
		printf("Error: Invalid user type. (Must be: AA, FS, BS, SS, or AM).\n");
		newUser.accountType = "ER";
		return newUser;
	}

	// if account manager tries to create an admin or account manager
	if (newUser.accountType == "AA" || newUser.accountType == "AM") {
		if (this->accountType == "AM") {
			printf("Error: AM user type can only create regular users, not AA or AM.\n");
			newUser.accountType = "ER";
			return newUser;
		}
	}

	std:string password;
	std::printf("Enter a password:\n");
	std::cin >> password;
	std::cin.ignore();

	// if input is "cancel"
	if (cancelCommandEntered(password)) {
		newUser.accountType = "ER";
		return newUser;
	}

	if (password.length() > 15 || password.length() < 1) {
		printf("Error: Password be only 15 characters in max length.\n");
		newUser.accountType = "ER";
	} else {
		newUser.password = Encrypt(password);
	}

	return newUser;
}

// delete to users account
std::string User::DeleteAccount() {
	std::string username;
	std::printf("Enter a username to delete:\n");
	std::cin >> username;

	return username;
}

// advertise items and checks thersholds
ITEM_RECORD User::Advertise() {
	ITEM_RECORD itemRecord;		// Initialize new ITEM_RECORD for newly created item
	std::string minBid;
	std::string duration;

	// User input for minimum bid for advertise operation
	std::printf("Enter the minimum bid ($):\n");
	std::cin >> minBid;
	std::cin.clear();
	std::cin.ignore();

	// if the input is "cancel"
	if (cancelCommandEntered(minBid)) {
		// set itemRecord duration to 999 (which is the MAX_DURATION)
		// so that when this is returned to main.cpp, the item is NOT created
		itemRecord.duration = 999;
		return itemRecord;
	}

	if (is_number(minBid) == false) { // Checking if min bid amount is a number
		itemRecord.duration = 999;
		std::printf("Error: Minimum bid amount must be a number.\n");
		return itemRecord; 
	} else { // Checking if min bid amount is valid
		if (std::stoi(minBid) > 999.99 || std::stoi(minBid) < 0) { 
			itemRecord.duration = 999;
			std::printf("Error: Minimum bid amount must be betwceen 0-999.99.\n");
			return itemRecord;
		} else {
			itemRecord.minBid = std::stoi(minBid);
		}
	}
	

	// User input for duration for advertise operation
	std::printf("Enter auction duration (in days):\n");
	std::cin >> duration;
	std::cin.clear();
	std::cin.ignore();

	// if the input is "cancel"
	if (cancelCommandEntered(duration)) {
		// set itemRecord duration to 999 (which is the MAX_DURATION)
		// so that when this is returned to main.cpp, the item is NOT created
		itemRecord.duration = 999;
		return itemRecord;
	}

	if (is_number(duration) == false) { // Checking if duration is a number
		itemRecord.duration = 999;
		std::printf("Error: Duration must be a number.\n");
		return itemRecord; 
	} else { // Checking if duration is valid
		if (std::stoi(duration) > 100 || std::stoi(duration) < 1) { 
			itemRecord.duration = 999;
			std::printf("Error: Auction duration must be between 1-100 days.\n");
			return itemRecord;
		} else {
			itemRecord.duration = std::stoi(duration);
		}
	}

	return itemRecord;
}

// refund to users account, and checks thersholds
REFUND_RECORD User::Refund() {
	REFUND_RECORD refundRecord; // Initialize new REFUND_RECORD

	// Asking user for inputs to run refund operation
	std::string amount;
	std::printf("Enter refund amount:\n");
	std::cin >> amount; 

	if (!cancelCommandEntered(amount)) {
		if (isFloat(amount)) {
			if (stof(amount) <= 0) {
				printf("Error: Refund amount must be a greater than 0.\n");
				refundRecord.buyer.empty();
				refundRecord.seller.empty();
				refundRecord.amount = 0;
			} else {
				refundRecord.amount = stof(amount);
			}
		} else {
			printf("Error: Refund amount must be a number.\n");
			refundRecord.buyer.empty();
			refundRecord.seller.empty();
			refundRecord.amount = 0;
		}
	}
	else {
		refundRecord.buyer.empty();
		refundRecord.seller.empty();
		refundRecord.amount = 0;
	}

	return refundRecord;
}

// addcredit to users account, and checks thersholds
float User::AddCredit(float amount) {

	if ((this->credit + amount) < MAX_CREDIT) {
		this->credit += amount;
	} else {
		std::printf("Error: Exceeded $%i credit limit for this user.\n", MAX_CREDIT); 
	}

	this->credit = round(this->credit * 100.0) / 100.0;		

	return this->credit;
}

// resets password and checks thersholds
std::string User::ResetPassword() {
	std:string password;
	std::printf("Enter a password:\n");
	std::cin >> password;
	std::cin.ignore();

	if (!cancelCommandEntered(password)) {
		if (password.length() > 15 || password.length() < 1) {
			printf("Error: Password must be between 1-15 characters long.\n");
			password = password.empty();
		} else {
			password = Encrypt(password);

			if (this->password == password) {
				printf("This is already the password! Returning to main menu\n");

				// return "" so that when this is returned to main.cpp, the password
				// is not changed and also the transaction is not completed/added to 
				// the daily transaction file
				return "";
			}
			else {
				this->password = password;
				printf("Successfully changed password!\n");
			}
		}
	}

	return password;
}

// Validate if input is a number
bool User::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
	// Checking if each char is a digit
    while (it != s.end() && isdigit(*it)) {
		++it;
	} 
    return !s.empty() && it == s.end();
}

// Validate if input is a float type
bool User::isFloat(const std::string& s) {
    std::istringstream iss(s);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail(); 
}

// Check if input is "cancel" (meaning the user wants to cancel the current transaction)
bool User::cancelCommandEntered(const std::string& s) {
	if (s == "canceltransaction") {
		cout << "Cancelling current transaction and returning to main menu\n" << endl;
		return true;
	}

	return false;
}
