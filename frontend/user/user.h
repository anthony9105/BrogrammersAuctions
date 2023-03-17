#include <string>
using namespace std;

#ifndef USER_H
#define USER_H

/**
 * User class
*/
class User {
    protected:
        double balance = 0.0;
        string name;
        string accountType;
        string password;

    public:
        // constructor
        User(string iName, string iAccountType, string iPassword);

        User() {};
};

#endif