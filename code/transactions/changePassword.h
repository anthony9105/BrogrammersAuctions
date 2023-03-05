#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD

using namespace std;

class ChangePassword : public Transaction {
    public :
        void executeTransaction(string name, string accountType, int balance, string currentPassword);
};

#endif