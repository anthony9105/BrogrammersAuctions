#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include "../user/user.h"
#include "transaction.h"
#include "changePassword.h"

void ChangePassword::executeTransaction(string name, string accountType, int balance, string currentPassword) {
    string newPassword;
    bool validPassword= false;

    while (!validPassword) {
        cout << "Enter new password (more than 1 character and less than 15 characters)" << endl;
        getline(cin, newPassword);

        // if over 1 character, less than 15 characters, and not the same as the current password
        if (newPassword.length() > 1 && newPassword.length() < 15 && newPassword != currentPassword) {
            validPassword = true;
        } 
    }

    Transaction::updatePasswordInUsersFile(name, newPassword);
    Transaction::addToTransFile(name, accountType, balance, "07");

    cout << "Password updated successfully" << endl;
}