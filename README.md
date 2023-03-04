# BrogrammersAuctions

Steps to compile and run in a terminal:

1. make sure you are in the code directory
2. enter:  g++ main.cpp user/user.cpp transactions/transaction.cpp transactions/create.cpp transactions/delete.cpp transactions/addcredit.cpp transactions/changePassword.cpp -o main
3. enter: ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt
4. the previous step wants the path to the DailyTransactions.txt file first and then the CurrentUserAccounts.txt file after

*To cancel out of a transaction enter exactly: cancel please*

Mutliple terminals can run ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt at the same time and work since the transaction files and current user accounts files are updated immediately rather than just after logout.

If CurrentUserAccounts.txt file path is incorrect you will not be able to login since it is reading from a non-existing file to check for username and password.

For phase 2 files: advertise.cpp, advertise.h, CurrentItemsFileManager.cpp, CurrentItemsFileManager.h, refund.cpp, refund.h, item.cpp, item.h can be ignored as they are not implemented or used right now