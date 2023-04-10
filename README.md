# BrogrammersAuctions

Steps to compile and run in a terminal:

1. make sure you are in the frontend directory
2. enter:  g++ main.cpp user/user.cpp transactions/transaction.cpp transactions/create.cpp transactions/delete.cpp transactions/addcredit.cpp transactions/changePassword.cpp -o main
3. enter: ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt ../files/AvailableItemsFile.txt
4. the previous step wants the path to the DailyTransactions.txt file first and then the CurrentUserAccounts.txt file after

*To cancel out of a transaction enter exactly: cancel please*

Mutliple terminals can run ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt ../files/AvailableItemsFile.txt at the same time and work since the transaction files and current user accounts files are updated immediately rather than just after logout.

To run the test script:
1. make sure you are in the frontend directory
2. run chmod +x testing.sh
3. run ./testing.sh

To run the backend prototype:
1. make sure you are in the backend/src directory
2. run python backend.py or python3 backend.py

To run the backend tests:
1. make sure you are in the backend/src directory
2. run pytest

To run the weekly script:

1. make sure you are in the front_end_1/src directory
2. run chmod +x daily01.sh
3. run chmod +x daily02.sh
4. run chmod +x daily03sh
5. run chmod +x daily04.sh
6. run chmod +x daily05.sh
7. run chmod +x daily01_content.sh
8. run chmod +x daily02_content.sh
9. run chmod +x daily03_contentsh
10. run chmod +x daily04_content.sh
11. run chmod +x daily05_content.sh
12. run chmod +x weekly.sh
13. run ./weekly.sh
