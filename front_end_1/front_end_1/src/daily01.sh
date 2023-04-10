#!/usr/bin/expect -f

spawn ./auction_system ./current_users_accounts.txt ./available_items.txt ./iofiles/daily_transaction.txt
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
