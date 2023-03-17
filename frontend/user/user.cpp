#include <string>
#include "user.h"

using namespace std;

// constructor
User::User(string iName, string iAccountType, string iPassword) {
    name = iName;
    accountType = iAccountType;
    password = iPassword;
}