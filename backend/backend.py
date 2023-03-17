# The backend will first make a new file which has the new lines of the daily transaction files. It will store each line in a list for easy reading and access.
# The program then goes through this lists and makes the changes depending on the contents of the line and will use the starting number of the line to know 
# which function to call for each of the different commands that are in the frontend.

# This function makes a new file which has the new lines of the daily transaction files.
def createDailyTransactionNewContent():
    with open("daily_transaction.txt", "r") as a_file:
        a_lines = set(a_file.readlines())

    with open("merged_daily_transaction.txt", "r") as b_file, open("daily_transaction_new_content.txt", "w") as c_file:
        for line in b_file:
            if line not in a_lines:
                c_file.write(line)

# This function makes the modifications to the current_users_accounts.txt file that the create command causes.
def create_modifications(words):
    with open("current_users_accounts.txt", "a") as file:
                file.write(words[1] + " " + words[2] +
                           " " + words[3] + " " + words[4] + "\n")

# This function makes the modifications to the current_users_accounts.txt file that the delete command causes.
def delete_modifications(words):
    with open("current_users_accounts.txt", "r") as file:
                lines = file.readlines()

    with open("current_users_accounts.txt", "w") as file:
        for line in lines:
            if line.strip() != (words[1] + " " + words[2] +
            " " + words[3] + " " + words[4]):
                file.write(line)

# This function makes the modifications to the available_items.txt file that the advertise command causes.
def advertise_modifications(words):
    with open("available_items.txt", "a") as file:
                file.write(words[1] + " " + words[2] +
                           " N/A " + words[3] + " " + words[4] + "\n")
                
# This function makes the modifications to the available_items.txt file that the bid command causes.
def bid_modifications(words):
    with open("available_items.txt", "r") as file:
                lines = file.readlines()

    with open("available_items.txt", "w") as file:
        for line in lines:
            if line.startswith(words[1]):
                file.write(words[1] + " " + words[2] +
                    " user01 " + words[3] + " " + words[4] + "\n") #" user01 " is a placeholder
            else:
                file.write(line)

    with open("current_users_accounts.txt", "r") as file:
                lines2 = file.readlines()

    with open("current_users_accounts.txt", "w") as file:
        for line in lines2:
            if line.startswith("user01"):  # "user01" is a placeholder
                words2 = line.split()
                if len(words2) >= 3 and words2[2].isdigit():
                    words2[2] = str(int(words2[2]) - int(words[4]))
                new_line = " ".join(words2) + "\n"
                file.write(new_line)

            else:
                file.write(line)

# This function makes the modifications to the current_users_accounts.txt file that the refund command causes.
def refund_modifications(words):
    with open("current_users_accounts.txt", "r") as file:
        lines = file.readlines()

    with open("current_users_accounts.txt", "w") as file:
        for line in lines:
            if line.startswith(words[1]):
                words2 = line.split()
                if len(words2) >= 3 and words2[2].isdigit():
                    words2[2] = str(int(words2[2]) + 3)
                new_line = " ".join(words2) + "\n"
                file.write(new_line)

            elif line.startswith(words[2]):
                words2 = line.split()
                if len(words2) >= 3 and words2[2].isdigit():
                    words2[2] = str(int(words2[2]) - 3)
                new_line = " ".join(words2) + "\n"
                file.write(new_line)

            else:
                file.write(line)

# This function makes the modifications to the current_users_accounts.txt file that the addcredit command causes.
def addCredit_modifications(words):
    with open("current_users_accounts.txt", "r") as file:
        lines = file.readlines()

    with open("current_users_accounts.txt", "w") as file:
        for line in lines:
            if line.startswith(words[1]):
                file.write(words[1] + " " + words[2] +
                            " " + words[3] + " " + words[4] + "\n")
            else:
                file.write(line)

# This function makes the modifications to the current_users_accounts.txt file that the resetpassword command causes.
def resetPassword_modifications(words):
    with open("current_users_accounts.txt", "r") as file:
        lines = file.readlines()

    with open("current_users_accounts.txt", "w") as file:
        for line in lines:
            if line.startswith(words[1]):
                file.write(words[1] + " " + words[2] +
                            " " + words[3] + " " + words[4] + "\n")
            else:
                file.write(line)

# This function will do the back end error recording. It is currently incomplete. 
def log_information():
    # TODO
    return 0

# This is where the main functionality begins
createDailyTransactionNewContent()

with open("daily_transaction_new_content.txt", "r") as c_file:
    for line in c_file:
        words = line.split()

        if words[0] == '01':
            create_modifications(words)

        elif words[0] == '02':
            delete_modifications(words)

        elif words[0] == '03':
            advertise_modifications(words)

        elif words[0] == '04':  
            bid_modifications(words)

        elif words[0] == '05':
            refund_modifications(words)

        elif words[0] == '06':
            addCredit_modifications(words)

        elif words[0] == '07':
            resetPassword_modifications(words)
