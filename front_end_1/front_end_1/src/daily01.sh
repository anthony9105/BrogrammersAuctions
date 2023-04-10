#!/usr/bin/expect -f

spawn ./auction_system ./current_users_accounts.txt ./available_items.txt ./iofiles/available_items.txt ./iofiles/daily_transaction
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

spawn ./auction_system ./current_users_accounts.txt ./available_items.txt ./iofiles/available_items.txt ./iofiles/daily_transaction2
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

spawn ./auction_system ./current_users_accounts.txt ./available_items.txt ./iofiles/available_items.txt ./iofiles/daily_transaction3
expect "Enter operation:"
send "login\r"
send "user01\r"
send "pass\r"
send "delete\r"
send "logout\r"
send "close\r"
interact
