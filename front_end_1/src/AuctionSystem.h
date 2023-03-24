#ifndef AUCTIONSYSTEM_H
#define AUCTIONSYSTEM_H

#include "Record.h"
#include "User.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class AuctionSystem {	
	public:
		bool loggedIn; // States if a user is logged into the auction system (true/false)
		bool isOpen;   // States if the auction system program should close after next command

		// Constructor
		AuctionSystem(bool auctionStatus);

		// Public methods
		bool Login(USER_RECORD userRecord, User &currentUser);
		void Logout(User &currentUser);

};

string Encrypt(string password);
string Decrypt(string password);

#endif