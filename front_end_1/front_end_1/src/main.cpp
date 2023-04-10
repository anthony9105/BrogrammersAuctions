/* Program: Brogrammer's Auctions
 * Description: Auction-style Sales Service console application.

	Takes in the current users, available items file, and daily transaction file name/path to create.
	Full running instructions found in front_end_1/front_end_1/src/RunningInstructions.txt
*/

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <limits>
#include <cmath>
#include <memory>
#include <vector>
#include <string.h>

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

	std::string command;				// Stores user action (input)
	std::string transactionCode;		// Stores the current user actions transaction code (if any)
	std::string transactionDetails;  // Stores the transaction log (formatted information to append to transaction history)

	// Initialize Objects
	FileController fc(argv[1], argv[2], argv[3]);  // Takes in all the input files (accoutns file, items file, and transaction file)
	AuctionSystem auctionSys(true);				   // Initializing auction system by calling AuctionSystem class
	User currentUser;                              // Creating new user object

	// if "reset" is given as the 5th command line argument, reset the merged daily transaction file
	if (argc >= 5) {
		if (strcmp(argv[4], "reset") == 0) {
			fc.resetMergedTransFile();
		}
	}

	std::cout << "Welcome to Brogrammer's Auctions!\n--" << std::endl;
    std::cout << "Here is a list of operations (what to input):" << std::endl;
    std::cout << "- Login (login)\n"
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
		 << "- Cancel auction (cancel)\n" 
		 << "- Cancel transaction [only when in the middle of a transaction] (canceltransaction)\n" << std::endl;

	// Repeat until auction closes (exit command sets auction to closed)
	while (auctionOpen) {
		transactionCode.clear(); // Reset the transaction code

		currentUser.credit = round(currentUser.credit * 100.0f) / 100.0f;

		// Prompt user to log in (repeat until user is logged in)
		while (!auctionSys.loggedIn) {
			std::string userInput;
			printf("Enter operation:\n");
			getline(std::cin, userInput);

			if (userInput == "login") {
				std::string username;
				std::printf("Enter username:\n");
				getline(std::cin, username);

				if (!currentUser.cancelCommandEntered(username)) {
					USER_RECORD userRecord = fc.getUser(username); // Search for username in the users file
					if (!userRecord.username.empty()) {
						auctionSys.Login(userRecord, currentUser);
					} else {
						std::cout << "Error: This username does not exist in the user-accounts file.\n" << std::endl;
					}
				}
			}
			else {
				std::cout << "Error: You must login first.\n" << std::endl;
			}
		}

		// 	Asks user for new input of operation everytime until they logout
		printf("\nEnter operation:\n");
		getline(std::cin, command);
		transform(command.begin(), command.end(), command.begin(), ::tolower); // Transform command entered to lower-case

		// User enters Create operation - Create a new user account
		if (command == "create") {
			if (currentUser.accountType == ADMIN) {

				std::string username;
				std::printf("Enter a username to create:\n");
				std::cin >> username;

				if (currentUser.cancelCommandEntered(username)) {
					std::cin.ignore();
				}
				else if (username.length() > 15) {
					cout << "Error: Username must be only 15 characters in max length." << endl;
					std::cin.ignore();
				} else if (fc.findUser(username)) {
					printf("Error: This username already exists in the user-accounts file.\n");
					std::cin.ignore();
				} else {
					USER_RECORD newUserRecord = currentUser.CreateAccount(); // Create a new user record

					if (newUserRecord.accountType != "ER") {                 // Validating user privilege, making sure it is not default
						newUserRecord.username = username;

						//fc.addUser(newUserRecord);			// Add the new user record to the current users file

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
			if (currentUser.accountType == ADMIN || currentUser.accountType == ACCOUNT_MANAGER) { // Verifies user access to operation
				std::string username = currentUser.DeleteAccount();

				if (!currentUser.cancelCommandEntered(username)) {
					USER_RECORD userRecord = fc.getUser(username);  // Retreive user record or user to delete

					if (!userRecord.username.empty()) {
						if (userRecord.username != currentUser.username) {

							// if the current user is an AM, they cannot delete AA or AM, only FS, BS, or SS
							if (currentUser.accountType == ACCOUNT_MANAGER && (userRecord.accountType == ADMIN || userRecord.accountType == ACCOUNT_MANAGER)) {
								printf("Error: As an AM user, you can only delete regular users (FS, BS, SS) and not AA or AM\n");
							}
							else {
								fc.deleteUser(username);		// Remove the user record

								std::printf("%s successfully deleted.\n", username.c_str());
								// Add delete record to transaction file
								transactionCode = DELETE_TRANSACTION_CODE;
								transactionDetails = recordToString(userRecord);
							}
						} else { // Checks if user tries to delete itself
							std::printf("Error: You cannot delete your own user.\n");
						}
					} else { // Checks if user is in the user-accounts file
						std::printf("Error: This username does not exist in the user-accounts file.\n");
					}
				}
				std::cin.ignore();
			} else {
				std::cout << "Error: Only AA and AM has privileges for this operation." << std::endl;
			}
		}

		// User enters Create operation - Put an item up for auction
		else if (command == "advertise") {
			if (currentUser.accountType == BUY_STANDARD) { 		// Validating user privilege, making sure it is not a buy standard account
				std::printf("Error: Only AA, FS, or SS have privileges for this operation.\n");
			} else {
				// User input for item name for advertise operation
				std::string itemName;
				std::printf("Enter the item name (no spaces):\n");
				std::cin >> itemName;

				// clearing cin completely incase an item name which is 2 or more words entered does not
				// carry over and mess up the next part.  Example: soccer ball   is entered, only "soccer"
				// is taken as the itemName.
				std::cin.clear();
				std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// if "cancel" is not entered
				if (!currentUser.cancelCommandEntered(itemName)) {
					// Checking if item name is valid
					if (itemName.length() > 25 || itemName.length() < 1) {
						std::printf("Error: Item name must be between 1-25 characters in length.\n");
					} else if (fc.findItem(itemName)) {
						std::printf("Error: %s already exists in available-items file.\n", itemName.c_str());
					} else {
						ITEM_RECORD itemRecord = currentUser.Advertise();

						if (itemRecord.duration != MAX_DURATION) {
							itemRecord.itemName = itemName;
							itemRecord.seller = currentUser.username;	   // Set the seller name to the current user
							itemRecord.buyer = "N/A";					   // Default buyer name
							fc.addItem(itemRecord);					       // Add the new item record to the available items file
							std::printf("%s successfully put up for auction for %d days.\n", itemRecord.itemName.c_str(), itemRecord.duration);

							transactionCode = ADVERTISE_TRANSACTION_CODE;
							transactionDetails = recordToString(itemRecord);
						}
					}
				}
			}
		}

		// User enters bid operation - Bid on an item up for auction
		else if (command == "bid") {
			if (currentUser.accountType == SELL_STANDARD) {			// Validating user privilege, making sure it is not a sell standard account
				std::printf("Error: Only AA, FS, or BS have privileges for this operation.\n");
			} else {
				std::string itemName;
				std::string seller;

				// ITEM_RECORD itemRecord = currentUser.Bid(); 	  // Create item record for item to bid on
				std::printf("Enter item name:\n");
				std::cin >> itemName;
				std::cin.clear();
				std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// if the input is NOT "cancel"
				if (!currentUser.cancelCommandEntered(itemName)) {
					if (!fc.findItem(itemName)) {
						std::printf("Error: %s does not exist in available-items file.\n", itemName.c_str());
					} else {
						std::printf("Enter seller's username:\n");
						std::cin >> seller;

						std::cin.clear();
						std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

						// if the input is NOT "cancel"
						if (!currentUser.cancelCommandEntered(seller)) {
							if (!fc.findUser(seller)) {
								std::printf("Error: This username does not exist in the user-accounts file.\n");
							} else if (seller == currentUser.username) {
								std::printf("Error: You cannot place a bid on your own advertised item.\n");
							} else {
								ITEM_RECORD itemRecord = fc.getItem(itemName);
								if (itemRecord.seller != seller) { // Check if the seller is correct for the inputted item name
									std::printf("Error: This user does not sell this item.\n");
								} else {
									if (itemRecord.duration > -1) {
										float highestBid = itemRecord.highestBid; // Find item and the highest bid currently on it (return -1 if item not found)
										if (highestBid > -1) {
											std::string bidAmount;
											std::cout << "Current Highest Bid: $" << highestBid << std::endl;
											std::printf("Enter new bid amount:\n");
											std::cin >> bidAmount;

											std::cin.clear();
											std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
											
											// if input is NOT "cancel"
											if (!currentUser.cancelCommandEntered(bidAmount)) {
												if (!currentUser.isFloat(bidAmount)) {
													std::printf("Error: Bid amount must be a number.\n");
												} else {
													if (stof(bidAmount) > currentUser.credit) {
														std::printf("Error: You do not have enough credits to complete this bid.\n");
													} else if (currentUser.accountType == ADMIN && stof(bidAmount) <= highestBid) {
														std::printf("Error: Bid amount must be greater than current highest bid.\n");
													} else if (currentUser.accountType != ADMIN && stof(bidAmount) <= (highestBid*0.05 + highestBid)) {
														std::printf("Error: Bid amount must be 5%% greater than current highest bid.\n");
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
														std::printf("Bid of $%.2f successfully placed on %s.\n", stof(bidAmount), itemRecord.itemName.c_str());

														transactionCode = BID_TRANSACTION_CODE;
														transactionDetails = recordToString(itemRecord);
													}
												}
											}
										}
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
			if (currentUser.accountType == ADMIN || currentUser.accountType == ACCOUNT_MANAGER) {
				fc.displayAvailableUsers(); // Displays and formats current users file
			} else {
				std::printf("Error: Only AA and AM has privilages for this operation.\n");
			}
		}

		else if (command == "refund") {
			// Validating user privilege, making sure it is an admin
			if (currentUser.accountType == FULL_STANDARD || currentUser.accountType == BUY_STANDARD || currentUser.accountType == SELL_STANDARD) {
				std::printf("Error: Only AA and AM has privileges for this operation.\n");
			} else {
				REFUND_RECORD refundRecord = currentUser.Refund();	// Create refund record for buyer to get refund from seller

				if (refundRecord.amount > 0) {
					std::printf("Enter buyer's username:\n");
					std::cin >> refundRecord.buyer;

					// if input is NOT "cancel"
					if (!currentUser.cancelCommandEntered(refundRecord.buyer)) {
						// check if buyer username exists
						if (!fc.findUser(refundRecord.buyer)) {
							std::printf("Error: This username does not exist in the user-accounts file.\n"); // TO-DO: fix error
						} else {
							std::cin.clear();
							std::printf("Enter seller's username:\n");
							std::cin >> refundRecord.seller;

							// if input is NOT "cancel"
							if (!currentUser.cancelCommandEntered(refundRecord.seller)) {
								// check if seller username exists
								if (!fc.findUser(refundRecord.seller)) {
									std::printf("Error: This username does not exist in the user-accounts file.\n"); // TO-DO: fix error
								} else {
									USER_RECORD buyerRecord = fc.getUser(refundRecord.buyer);
									USER_RECORD sellerRecord = fc.getUser(refundRecord.seller);

									if (buyerRecord.credit + refundRecord.amount > MAX_CREDIT) {
										std::printf("Error: Buyer's credit balance cannot be more than $999,999.00.\n"); // TO-DO: fix error
									} else if (sellerRecord.credit - refundRecord.amount < MIN_CREDIT) {
										std::printf("Error: Seller's credit balance cannot be less than $0.\n"); // TO-DO: fix error
									} else {
										buyerRecord.credit += refundRecord.amount;  // Add the refund amount to the buyers account
										sellerRecord.credit -= refundRecord.amount; // Remove the refund amount from the sellers account

										fc.updateCredit(buyerRecord.username, buyerRecord.credit);   // Update the buyers credit record
										fc.updateCredit(sellerRecord.username, sellerRecord.credit); // Update the sellers credit record

										std::printf("$%.2f refunded from %s to %s\n", refundRecord.amount, refundRecord.seller.c_str(), refundRecord.buyer.c_str());
										transactionCode = REFUND_TRANSACTION_CODE;
										transactionDetails = recordToString(refundRecord);
									}
								}
							}
						}
					}
				}
				cin.ignore();
			}
		}

		// User enters addcredit operation - Add a credit amount to a user
		else if (command == "addcredit") {
			std::printf("Current credit balance for %s: $%.2f\n", currentUser.username.c_str(), currentUser.credit);
			std::string amount;
			std::cout << "Enter the credit amount to add:\n";
			std::cin >> amount;
			std::cin.ignore();

			// if input is NOT "cancel"
			if (!currentUser.cancelCommandEntered(amount)) {
				if (!currentUser.isFloat(amount)) {
					std::printf("Error: Credit amount must be a number.\n");
				} else {
					if (stof(amount) > MAX_CREDIT_ADD || stof(amount) < 1) {
						std::printf("Error: Invalid input for credit. Enter a number between 1-1000.\n");
					} else {
						if (currentUser.accountType == ADMIN) {
							std::string username;
							std::cout << "Enter the username to add credit to:\n";
							std::cin >> username;
							std::cin.ignore();

							// if input is NOT "cancel"
							if (!currentUser.cancelCommandEntered(username)) {
								if (!fc.findUser(username)) {
									std::printf("Error: This username does not exist in the user-accounts file.\n");
								} else {
									USER_RECORD userRecord = fc.getUser(username);
									float newCredit;
									if (userRecord.username == currentUser.username) {
										newCredit = currentUser.AddCredit(stof(amount));
										userRecord.credit = newCredit;
										fc.updateCredit(username, newCredit); // Update users credit

										transactionCode = ADDCREDIT_TRANSACTION_CODE;
										transactionDetails = currentUser.toString();
										std::printf("New credit balance for %s: $%.2f\n", username.c_str(), newCredit);
									} else {
										if ((userRecord.credit + stof(amount)) < MAX_CREDIT) {
											newCredit = userRecord.credit + stof(amount);
											userRecord.credit = newCredit;
											fc.updateCredit(username, newCredit); // Update users credit

											transactionCode = ADDCREDIT_TRANSACTION_CODE;
											transactionDetails = currentUser.toString();
											std::printf("New credit balance for %s: $%.2f\n", username.c_str(), newCredit);
										} else {
											std::printf("Error: Exceeded $%i credit limit for this user.\n", MAX_CREDIT);
										}
									}
								}
							}
						} else {
							float newCredit = currentUser.AddCredit(stof(amount));
							fc.updateCredit(currentUser.username, newCredit); // Update users credit
							transactionCode = ADDCREDIT_TRANSACTION_CODE;
							transactionDetails = currentUser.toString();
							std::printf("New credit balance for %s: $%.2f\n", currentUser.username.c_str(), currentUser.credit);
						}
					}
				}
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
			std::cout << "Error: You are already logged in." << std::endl;
		}

		// User enters logout operation - Close the current users session
		else if (command == "logout") {
			transactionCode = LOGOUT_TRANSACTION_CODE;
			transactionDetails = currentUser.toString(); // Write logout to daily transaction
			auctionOpen = false;			 // Bool changes to false as system closes
			auctionSys.Logout(currentUser);  // Logout user from auction system
			currentUser = User();            // Reset current user information
		}

		else if (command == "cancel") {
			if (currentUser.accountType == ADMIN || currentUser.accountType == ACCOUNT_MANAGER) {
				std::string itemName;
				cout << "Enter the item name to cancel the auction for: " << endl;
				std::cin >> itemName;

				std::cin.clear();
				std::cin.ignore();

				if (fc.findItem(itemName)) {
					ITEM_RECORD itemRecord = fc.getItem(itemName);
					transactionCode = CANCELAUCTION_TRANSACTION_CODE;
					transactionDetails = recordToString(itemRecord);

					printf("Auction for %s, by seller: %s, successfully cancelled.\n", itemRecord.itemName.c_str(), itemRecord.seller.c_str());
				}
				else {
					std::printf("Error: %s does not exist in available-items file.\n", itemName.c_str());
				}

			}
			else {
				cout << "Error: Only AA and AM has privilages for this operation." << endl;
			}
		}

		// Handles any case where user inputs any operation other than the above conditionals
		else {
			cout << "Invalid input for operation. Please try again." << endl;
		}

		// print to daily transaction file
		if (!transactionCode.empty()) {
			fc.logTransaction(transactionCode, transactionDetails); // Log all daily transactions

			// update merged transaction file
			std::string pathToTransFiles = "./iofiles";
			ofstream outfile;
			outfile.open("./iofiles/merged_daily_transaction.txt", ios_base::app);

			ifstream infile;
			infile.open(fc.getTransFile());

			string contents;

			if (infile) {
				ostringstream ss;
				ss << infile.rdbuf(); // reading data
				contents = ss.str();
			}

			outfile << contents;

			outfile.close();
			infile.close();
		}
	}
    return 0;
}
