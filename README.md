# Brogrammer's Auctions
by Baron, Anthony, Kaatihan

# Steps to compile and run frontend in a terminal:
1. make sure you are in the front_end_1/front_end_1/src directory
2. enter: make
3. enter: ./auction_system ../../../backend/current_users_accounts.txt ../../../backend/available_items.txt ./iofiles/daily_transaction1.txt reset
4. the previous step takes the currentusers file first, then the availableitems file, then the transaction file to create, and optional argument "reset" which tells
the frontend to reset the merged daily transaction file to empty or not.

# Front end operations list:
- Login (login)
- Create (create)
- Delete (delete)
- Advertise (advertise)
- Bid (bid)
- Refund (refund)
- Add Credit (addcredit)
- List All Items (listallitems)
- List All Users (listallusers)
- Reset Password (resetpassword)
- Logout (logout)
- Cancel auction (cancel)
- Cancel transaction [only when in the middle of a transaction] (canceltransaction)

note: canceltransaction was not apart of the requirements (it was an added feature), which allows
a user to cancel/exit out of a transaction they are in the middle of by entering: canceltransaction.
This is different from the cancel auction (cancel) command.

# Starting users:
- user01	AA	pass
- user02	FS	pass
- user03	BS	pass
- user04	AM	newpass
- user05  SS  pass


# Starting items:
- item name:       seller:      current bidder/buyer:      days left:      current bid:

- vicks            user01            user03                  10                 10
- fan              user01             N/A                    75                 75
- lotion           user01            user02                   5                8.23
- lamp             user02             user01                 50               58.23
- soccerball       user01             N/A                    30                 23


# To run the test script:
1. make sure you are in the frontend directory
2. run chmod +x testing.sh
3. run ./testing.sh

# To run the backend:
1. make sure you are in the backend/src directory
2. run python backend.py or python3 backend.py

# To run the backend tests:
1. make sure you are in the backend/src directory
2. run pytest

# To run the weekly script:
1. make sure you are in the front_end_1/src directory
2. run chmod +x daily01.sh
3. run chmod +x daily02.sh
4. run chmod +x daily03.sh
5. run chmod +x daily04.sh
6. run chmod +x daily05.sh
7. run chmod +x daily01_content.sh
8. run chmod +x daily02_content.sh
9. run chmod +x daily03_contentsh
10. run chmod +x daily04_content.sh
11. run chmod +x daily05_content.sh
12. run chmod +x callBackend.sh
13. run chmod +x weekly.sh
14. run ./weekly.sh

# To run a daily script:
1. make sure you are in the front_end_1/src directory
2. run chmod +x daily01.sh
3. run ./daily01.sh
This is the same for daily01-05
