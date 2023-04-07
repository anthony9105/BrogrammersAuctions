/* Program: AuctionBay
 * Description: Auction-style Sales Service console application.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <typeinfo>

#include "Record.h"			// DEFINES RECORD STRUCTS & FUNCTIONS, AND CONSTANTS
#include "FileController.h" // HANDLES ALL FILE CONTROL
#include "User.h"			// HANDLES USER ACTIONS (PROMPTS) AND CURRENT USER INFORMATION
#include "AuctionSystem.h"	// HANDLES MENU, SESSION (LOGIN, LOGOUT)

using namespace std;

/*
	Main - Auction System Program
	@param int argc: The number of arguments passed
	@param: char** argv: Array of input files (format: current users, available items, daily transaction)
*/
int main(int argc, char** argv){
	// Initialize Variables
	bool auctionOpen = true;	// Whether or not to close (Exit) the auction system
	bool userFound = false;		// Whether or not the user exists (for login)

	string command;				// Stores user action (input)
	string transactionCode;		// Stores the current user actions transaction code (if any)
	string transactionDetails;  // Stores the transaction log (formatted information to append to transaction history)

	// Initialize Objects
	FileController fc(argv[1], argv[2], argv[3]);  // Takes in all the input files (accoutns file, items file, and transaction file)
	AuctionSystem auctionSys(true);				   // Initializing auction system by calling AuctionSystem class
	User currentUser;                              // Creating new user object

	cout << "Welcome to AuctionBay!\n--" << endl;
    cout << "Here is a list of operations (what to input):" << endl;
    cout << "- Login (login)\n"
         << "- Create (create)\n"
         << "- Delete (delete)\n"
         << "- Advertise (advertise)\n"
         << "- Bid (bid)\n"
         << "- Refund (refund)\n"
         << "- Add Credit (addcredit)\n"
         << "- List All Items (listallitems)\n"
		 << "- List All Users (listallusers)\n"
		 << "- Reset Password (resetpassword)\n"
         << "- Logout (logout)\n" 
		 << "- Cancel transaction (cancel)\n" 
		 << "- Close program (close)\n" << endl;

	// Repeat until auction closes (exit command sets auction to closed)
	while (auctionOpen) {
		transactionCode.clear(); // Reset the transaction code
		// Prompt user to log in (repeat until user is logged in)
		while (!auctionSys.loggedIn) {
			string userInput;
			printf("Enter operation:\n");
			getline(cin, userInput);

			if (userInput == "login") {
				string username;
				printf("Enter username:\n");
				getline(cin, username);

				USER_RECORD userRecord = fc.getUser(username); // Search for username in the users file
				if (!userRecord.username.empty()) {
					auctionSys.Login(userRecord, currentUser);
				} else {
					cout << "Error: This username does not exist in the user-accounts file.\n" << endl;
				}
			} else if (userInput == "close") {
				return 0;
			} else {
				cout << "Error: You must login first.\n" << endl;
			}
		}

		// 	Asks user for new input of operation everytime until they logout
		printf("\nEnter operation:\n");
		getline(cin, command);
		transform(command.begin(), command.end(), command.begin(), ::tolower); // Transform command entered to lower-case

		// User enters Create operation - Create a new user account
		if (command == "create") {
			if (currentUser.accountType == ADMIN) {

				std:string username;
				std::printf("Enter a username to create:\n");
				std::cin >> username;

				if (username.length() > 15) {
					cout << "Error: Username must be only 15 characters in max length." << endl;
					std::cin.ignore();
				} else if (fc.findUser(username)) {
					printf("Error: This username already exists in the user-accounts file.\n");
					std::cin.ignore();
				} else {
					USER_RECORD newUserRecord = currentUser.CreateAccount(); // Create a new user record

					if (newUserRecord.accountType != "ER") {                 // Validating user privilege, making sure it is not default
						newUserRecord.username = username;
						fc.addUser(newUserRecord);			// Add the new user record to the current users file
						printf("%s (%s) successfully created.\n", newUserRecord.username.c_str(), newUserRecord.accountType.c_str());
						// Add create record to transaction file
						transactionCode = CREATE_TRANSACTION_CODE;
						transactionDetails = recordToString(newUserRecord);
					}
				}
			} else {
				cout << "Error: Only AA has privileges for this operation." << endl;
			}
		}

		// User enters Delete operation - Delete a user account
		else if (command == "delete") {
			if (currentUser.accountType == ADMIN) { // Verifies user access to operation
				string username = currentUser.DeleteAccount();
				USER_RECORD userRecord = fc.getUser(username);  // Retreive user record or user to delete

				if (!userRecord.username.empty()) {
					if (userRecord.username != currentUser.username) {
						fc.deleteUser(username);		// Remove the user record from the current users file
						printf("%s successfully deleted.\n", username.c_str());
						// Add delete record to transaction file
						transactionCode = DELETE_TRANSACTION_CODE;
						transactionDetails = recordToString(userRecord);
					} else { // Checks if user tries to delete itself
						printf("Error: You cannot delete your own user.\n");
					}
				} else { // Checks if user is in the user-accounts file
					printf("Error: This username does not exist in the user-accounts file.\n");
				}
				cin.ignore();
			} else {
				cout << "Error: Only AA has privileges for this operation." << endl;
			}
		}

		// User enters Create operation - Put an item up for auction
		else if (command == "advertise") {
			if (currentUser.accountType == BUY_STANDARD) { 		// Validating user privilege, making sure it is not a buy standard account
				printf("Error: Only AA, FS, or SS have privileges for this operation.\n");
			} else {
				// User input for item name for advertise operation
				std::string itemName;
				std::printf("Enter the item name:\n");
				std::cin >> itemName;
				cin.ignore();
				// Checking if item name is valid
				if (itemName.length() > 25 || itemName.length() < 1) {
					std::printf("Error: Item name must be between 1-25 characters in length.\n");
					std::cin.clear();
				} else if (fc.findItem(itemName)) {
					printf("Error: %s already exists in available-items file.\n", itemName.c_str());
					std::cin.clear();
				} else {
					ITEM_RECORD itemRecord = currentUser.Advertise();

					if (itemRecord.duration != MAX_DURATION) {
						itemRecord.itemName = itemName;
						itemRecord.seller = currentUser.username;	   // Set the seller name to the current user
						itemRecord.buyer = "N/A";					   // Default buyer name
						fc.addItem(itemRecord);					       // Add the new item record to the available items file
						printf("%s successfully put up for auction for %d days.\n", itemRecord.itemName.c_str(), itemRecord.duration);

						transactionCode = ADVERTISE_TRANSACTION_CODE;
						transactionDetails = recordToString(itemRecord);
					}
					cin.ignore();
				}
			}
		}

		// User enters bid operation - Bid on an item up for auction
		else if (command == "bid") {
			if (currentUser.accountType == SELL_STANDARD) {			// Validating user privilege, making sure it is not a sell standard account
				printf("Error: Only AA, FS, or BS have privileges for this operation.\n");
			} else {
				string itemName;
				string seller;

				// ITEM_RECORD itemRecord = currentUser.Bid(); 	  // Create item record for item to bid on
				std::printf("Enter item name:\n");
				std::cin >> itemName;

				if (!fc.findItem(itemName)) {
					printf("Error: %s does not exist in available-items file.\n", itemName.c_str());
					cin.ignore();
				} else {
					printf("Enter seller's username:\n");
					cin >> seller;
					if (!fc.findUser(seller)) {
						printf("Error: This username does not exist in the user-accounts file.\n");
						cin.ignore();
					} else if (seller == currentUser.username) {
						printf("Error: You cannot place a bid on your own advertised item.\n");
						cin.ignore();
					} else {
						ITEM_RECORD itemRecord = fc.getItem(itemName);
						if (itemRecord.seller != seller) { // Check if the seller is correct for the inputted item name
							printf("Error: This user does not sell this item.\n");
							cin.ignore();
						} else {
							if (itemRecord.duration > -1) {
								float highestBid = itemRecord.highestBid; // Find item and the highest bid currently on it (return -1 if item not found)
								if (highestBid > -1) {
									string bidAmount;
									cout << "Current Highest Bid: $" << highestBid << endl;
									printf("Enter new bid amount:\n");
									cin >> bidAmount;

									if (!currentUser.is_number(bidAmount)) {
										printf("Error: Bid amount must be a number.\n");
										cin.ignore();
									} else {
										if (stof(bidAmount) > currentUser.credit) {
											printf("Error: You do not have enough credits to complete this bid.\n");
										} else if (currentUser.accountType == ADMIN && stof(bidAmount) <= highestBid) {
											printf("Error: Bid amount must be greater than current highest bid.\n");
										} else if (currentUser.accountType != ADMIN && stof(bidAmount) <= (highestBid*0.05 + highestBid)) {
											printf("Error: Bid amount must be 5%% greater than current highest bid.\n");
										} else {
											if (itemRecord.buyer != "N/A") {
												USER_RECORD userRecord = fc.getUser(itemRecord.buyer);
												userRecord.credit += itemRecord.highestBid;
												fc.updateCredit(userRecord.username, userRecord.credit);
											}
											currentUser.credit -= stof(bidAmount);
											itemRecord.highestBid = stof(bidAmount);
											fc.updateCredit(currentUser.username, currentUser.credit);
											fc.updateItemBid(itemRecord, currentUser.username);
											printf("Bid of $%.2f successfully placed on %s.\n", stof(bidAmount), itemRecord.itemName.c_str());

											transactionCode = BID_TRANSACTION_CODE;
											transactionDetails = recordToString(itemRecord);
										}
										cin.ignore();
									}
								}
							}
						}
					}
				}
			}
		}

		// User enters listall operation - List all all items in available items file
		else if (command == "listallitems") {
			fc.displayAvailableItems(); // Displays and formats current items file
		}

		else if (command == "listallusers") {
			if (currentUser.accountType == ADMIN) {
				fc.displayAvailableUsers(); // Displays and formats current users file
			} else {
				printf("Error: Only AA has privilages for this operation.\n");
			}
		}

		else if (command == "refund") {
			// Validating user privilege, making sure it is an admin
			if (currentUser.accountType == FULL_STANDARD || currentUser.accountType == BUY_STANDARD || currentUser.accountType == SELL_STANDARD) {
				printf("Error: Only AA has privileges for this operation.\n");
			} else {
				REFUND_RECORD refundRecord = currentUser.Refund();	// Create refund record for buyer to get refund from seller

				if (refundRecord.amount > 0) {
					std::printf("Enter buyer's username:\n");
					std::cin >> refundRecord.buyer;
					// check if buyer username exists
					if (!fc.findUser(refundRecord.buyer)) {
						printf("Error: This username does not exist in the user-accounts file.\n"); // TO-DO: fix error
					} else {
						std::cin.clear();
						std::printf("Enter seller's username:\n");
						std::cin >> refundRecord.seller;
						// check if seller username exists
						if (!fc.findUser(refundRecord.seller)) {
							printf("Error: This username does not exist in the user-accounts file.\n"); // TO-DO: fix error
						} else {
							USER_RECORD buyerRecord = fc.getUser(refundRecord.buyer);
							USER_RECORD sellerRecord = fc.getUser(refundRecord.seller);

							if (buyerRecord.credit + refundRecord.amount > MAX_CREDIT) {
								printf("Error: Buyer's credit balance cannot be more than $999,999.00.\n"); // TO-DO: fix error
							} else if (sellerRecord.credit - refundRecord.amount < MIN_CREDIT) {
								printf("Error: Seller's credit balance cannot be less than $0.\n"); // TO-DO: fix error
							} else {
								buyerRecord.credit += refundRecord.amount;  // Add the refund amount to the buyers account
								sellerRecord.credit -= refundRecord.amount; // Remove the refund amount from the sellers account

								fc.updateCredit(buyerRecord.username, buyerRecord.credit);   // Update the buyers credit record
								fc.updateCredit(sellerRecord.username, sellerRecord.credit); // Update the sellers credit record

								printf("$%.2f refunded from %s to %s\n", refundRecord.amount, refundRecord.seller.c_str(), refundRecord.buyer.c_str());
								transactionCode = REFUND_TRANSACTION_CODE;
								transactionDetails = recordToString(refundRecord);
							}
						}
					}
				}
				cin.ignore();
			}
		}

		// User enters addcredit operation - Add a credit amount to a user
		else if (command == "addcredit") {
			printf("Current credit balance for %s: $%.2f\n", currentUser.username.c_str(), currentUser.credit);
			std::string amount;
			std::cout << "Enter the credit amount to add:\n";
			std::cin >> amount;
			if (!currentUser.isFloat(amount)) {
				printf("Error: Credit amount must be a number.\n");
				cin.ignore();
			} else {
				if (stof(amount) > MAX_CREDIT_ADD || stof(amount) < 1) {
					printf("Error: Invalid input for credit. Enter a number between 1-1000.\n");
				} else {
					if (currentUser.accountType == ADMIN) {
						std::string username;
						std::cout << "Enter the username to add credit to:\n";
						std::cin >> username;
						if (!fc.findUser(username)) {
							printf("Error: This username does not exist in the user-accounts file.\n");
						} else {
							USER_RECORD userRecord = fc.getUser(username);
							float newCredit;
							if (userRecord.username == currentUser.username) {
								newCredit = currentUser.AddCredit(stof(amount));
								userRecord.credit = newCredit;
								fc.updateCredit(username, newCredit); // Update users credit in the current users file

								transactionCode = ADDCREDIT_TRANSACTION_CODE;
								transactionDetails = currentUser.toString();
								printf("New credit balance for %s: $%.2f\n", username.c_str(), newCredit);
							} else {
								if ((userRecord.credit + stof(amount)) < MAX_CREDIT) {
									newCredit = userRecord.credit + stof(amount);
									userRecord.credit = newCredit;
									fc.updateCredit(username, newCredit); // Update users credit in the current users file

									transactionCode = ADDCREDIT_TRANSACTION_CODE;
									transactionDetails = currentUser.toString();
									printf("New credit balance for %s: $%.2f\n", username.c_str(), newCredit);
								} else {
									std::printf("Error: Exceeded $%i credit limit for this user.\n", MAX_CREDIT);
								}
							}
						}
					} else {
						float newCredit = currentUser.AddCredit(stof(amount));
						fc.updateCredit(currentUser.username, newCredit); // Update users credit in the current users file

						transactionCode = ADDCREDIT_TRANSACTION_CODE;
						transactionDetails = currentUser.toString();
						printf("New credit balance for %s: $%.2f\n", currentUser.username.c_str(), currentUser.credit);
					}
				}
				cin.ignore();
			}
		}

		else if (command == "resetpassword") {
			std::string password;
			password = currentUser.ResetPassword();
			if (password == currentUser.password) {
				fc.resetPassword(currentUser.username, password); // Resets users password in the current users file
				transactionCode = RESETPASSWORD_TRANSACTION_CODE;
				transactionDetails = currentUser.toString();
			}
		}

		// User enters login operation - when already logged in (login02 test)
		else if (command == "login") {
			cout << "Error: You are already logged in." << endl;
		}

		// User enters logout operation - Close the current users session
		else if (command == "logout") {
			transactionCode = LOGOUT_TRANSACTION_CODE;
			transactionDetails = currentUser.toString(); // Write logout to daily transaction
			//auctionOpen = false;			 // Bool changes to false as system closes
			auctionSys.Logout(currentUser);  // Logout user from auction system
			currentUser = User();            // Reset current user information
			cout << "\n\n" << endl;
			//return 0;
		}

		// Handles any case where user inputs any operation other than the above conditionals
		else {
			cout << "Invalid input for operation. Please try again." << endl;
		}

		// print to daily transaction file
		if (!transactionCode.empty()) {
			fc.logTransaction(transactionCode, transactionDetails); // Log all daily transactions
		}
	}
    return 0;
}
