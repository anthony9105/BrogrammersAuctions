#!/usr/bin/expect -f

spawn ./auction_system ./iofiles/current_users_accounts.txt ./iofiles/available_items.txt ./iofiles/daily_transaction.txt
expect "Enter operation:"
send "login\r"
send "user01\r"
send "pass\r"
send "logout\r"
send "close\r"
interact
