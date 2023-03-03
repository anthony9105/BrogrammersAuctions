# BrogrammersAuctions
a

Steps to compile and run in a terminal:

1. make sure you are in the code directory
2. enter:  g++ main.cpp user/user.cpp transactions/transaction.cpp transactions/create.cpp transactions/delete.cpp transactions/addcredit.cpp transactions/changePassword.cpp -o main
3. enter: ./main

Side note: mutliple terminals can run ./main at the same time and work since the transaction files and current user accounts files are updated immediately rather than just after logout.

note: for phase 2 files: advertise.cpp, advertise.h, CurrentItemsFileManager.cpp, CurrentItemsFileManager.h, refund.cpp, refund.h, item.cpp, item.h can be ignored as they are not implemented or used right now