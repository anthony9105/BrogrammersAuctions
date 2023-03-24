#ifndef USER_H
#define USER_H

#include "Record.h"
#include <string>
#include <iostream>

class User {
	public:
		std::string username;
		std::string password;
		std::string accountType;
		float credit;

		std::string toString();

		USER_RECORD CreateAccount();
		std::string DeleteAccount();

		ITEM_RECORD Advertise();
		ITEM_RECORD Bid(std::string itemName, std::string seller);

		float AddCredit(float amount);
		REFUND_RECORD Refund();	
		std::string ResetPassword();

		bool is_number(const std::string& s);
		bool isFloat(const std::string& s);
		
};
#endif