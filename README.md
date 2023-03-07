# BrogrammersAuctions

Steps to compile and run in a terminal:

1. make sure you are in the code directory
2. enter:  g++ main.cpp user/user.cpp transactions/transaction.cpp transactions/create.cpp transactions/delete.cpp transactions/addcredit.cpp transactions/changePassword.cpp -o main
3. enter: ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt ../files/AvailableItemsFile.txt
4. the previous step wants the path to the DailyTransactions.txt file first and then the CurrentUserAccounts.txt file after

*To cancel out of a transaction enter exactly: cancel please*

Mutliple terminals can run ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt ../files/AvailableItemsFile.txt at the same time and work since the transaction files and current user accounts files are updated immediately rather than just after logout.

To run the test script:
1. make sure you are in the code directory
2. run chmod +x testing.sh
3. run ./testing.sh
