#include "AuctionSystem.h"
#include "User.h"
#include <algorithm>
#include <sstream>
#include <string>

// Initializes the Auction System class constructor
AuctionSystem::AuctionSystem(bool auctionStatus) {
	isOpen = auctionStatus;
	loggedIn = false;
}

bool AuctionSystem::Login(USER_RECORD userRecord, User &currentUser) {
	std::string password;
	printf("Enter password:\n");
	std::cin >> password;
	std::cin.ignore();

	if (password == Decrypt(userRecord.password)) {
		loggedIn = true; // Login method called once user logins in

		// Initializing all user information from current active login in session
		currentUser.username = userRecord.username;
		currentUser.password = userRecord.password;
		currentUser.accountType = userRecord.accountType;
		currentUser.credit = userRecord.credit;
		// Welcome user prompt
		std::cout << "Hello " << currentUser.username << " (" << currentUser.accountType << ")!" << std::endl;
	} else {
		std::cout << "Error: Incorrect password.\n" << std::endl;
	}

	return loggedIn;
}

// Logout method called once user logs out
void AuctionSystem::Logout(User &currentUser) {
	std::printf("%s successfully logged out.\n", currentUser.username.c_str());
	loggedIn = false;		// Reset loggedIn status once user logsout
}

// Encrypt users password to hash using Ceasar Cipher
std::string Encrypt(std::string password) {
	char ch;
	int i;
	for (i = 0; password[i] != '\0'; ++i) {
		ch = password[i];
		if(ch >= 'a' && ch <= 'z') {
			ch = ch + SALT;
			if(ch > 'z') {
				ch = ch - 'z' + 'a' - 1;
			}
			password[i] = ch;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			ch = ch + SALT;
			if (ch > 'Z') {
				ch = ch - 'Z' + 'A' - 1;
			}
			password[i] = ch;
		}
	}
	return password;
}

// Decrypt users password to validate user
std::string Decrypt(std::string password) {
	char ch;
	int i;
	for (i = 0; password[i] != '\0'; ++i) {
		ch = password[i];
		if (ch >= 'a' && ch <= 'z') {
			ch = ch - SALT;
			if (ch < 'a') {
				ch = ch + 'z' - 'a' + 1;
			}
			password[i] = ch;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			ch = ch - SALT;
			if (ch > 'a') {
			ch = ch + 'Z' - 'A' + 1;
			}
			password[i] = ch;
		}
	}
	return password;
}