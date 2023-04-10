#!/usr/bin/expect -f

spawn ./auction_system ../../../backend/current_users_accounts.txt ../../../backend/available_items.txt ./iofiles/daily_transaction1.txt reset
expect "Enter operation:"
send "login\r"
send "user01\r"
send "pass\r"
send "addcredit\r"
send "10\r"
send "user01\r"
send "logout\r"
send "close\r"
interact

spawn ./auction_system ../../../backend/current_users_accounts.txt ../../../backend/available_items.txt ./iofiles/daily_transaction2
expect "Enter operation:"
send "login\r"
send "user01\r"
send "pass\r"
send "resetpassword\r"
send "thisisaveryverylongpassword\r"
send "logout\r"
send "login\r"
send "user01\r"
send "thisisaveryverylongpassword\r"
send "resetpassword\r"
send "pass\r"
send "logout\r"
send "close\r"
interact

spawn ./auction_system ../../../backend/current_users_accounts.txt ../../../backend/available_items.txt ./iofiles/daily_transaction3
expect "Enter operation:"
send "login\r"
send "user01\r"
send "pass\r"
send "resetpassword\r"
send "thisisaveryverylongpassword\r"
send "logout\r"
send "login\r"
send "user01\r"
send "thisisaveryverylongpassword\r"
send "resetpassword\r"
send "pass\r"
send "logout\r"
send "close\r"
interact
