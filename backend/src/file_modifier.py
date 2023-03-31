# FileModifier class used to make the appropriate modifications to the file(s)
class FileModifier:
    # file paths
    curr_user_file = "../current_users_accounts.txt"
    avail_items_file = "../available_items.txt"

    # empty constructor
    def __init__(self):
        pass

    # This function makes the modifications to the current_users_accounts.txt file that the create command causes.
    def create_modifications(self, words):
        with open(self.curr_user_file, "a") as file:
                file.write(words[0] + " " + words[1] + " " + words[2] +
                        " " + words[3] + " " + words[4] + "\n")

    # This function makes the modifications to the current_users_accounts.txt file that the delete command causes.
    def delete_modifications(self, words):
        with open(self.curr_user_file, "r") as file:
                    lines = file.readlines()

        with open(self.curr_user_file, "w") as file:
            for line in lines:
                if line.strip() != (words[1] + " " + words[2] +
                " " + words[3] + " " + words[4]):
                    file.write(line)

    # This function makes the modifications to the available_items.txt file that the advertise command causes.
    def advertise_modifications(self, words):
        with open(self.avail_items_file, "a") as file:
                    file.write(words[1] + " " + words[2] +
                            " N/A " + words[3] + " " + words[4] + "\n")
                    
    # This function makes the modifications to the available_items.txt file that the bid command causes.
    def bid_modifications(self, words):
        with open(self.avail_items_file, "r") as file:
                    lines = file.readlines()

        with open(self.avail_items_file, "w") as file:
            for line in lines:
                if line.startswith(words[1]):
                    file.write(words[1] + " " + words[2] +
                        " user01 " + words[3] + " " + words[4] + "\n") #" user01 " is a placeholder
                else:
                    file.write(line)

        with open(self.curr_user_file, "r") as file:
                    lines2 = file.readlines()

        with open(self.curr_user_file, "w") as file:
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
    def refund_modifications(self, words):
        with open(self.curr_user_file, "r") as file:
            lines = file.readlines()

        with open(self.curr_user_file, "w") as file:
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
    def addCredit_modifications(self, words):
        with open(self.curr_user_file, "r") as file:
            lines = file.readlines()

        with open(self.curr_user_file, "w") as file:
            for line in lines:
                if line.startswith(words[1]):
                    file.write(words[1] + " " + words[2] +
                                " " + words[3] + " " + words[4] + "\n")
                else:
                    file.write(line)

    # This function makes the modifications to the current_users_accounts.txt file that the resetpassword command causes.
    def resetPassword_modifications(self, words):
        with open(self.curr_user_file, "r") as file:
            lines = file.readlines()

        with open(self.curr_user_file, "w") as file:
            for line in lines:
                if line.startswith(words[1]):
                    file.write(words[1] + " " + words[2] +
                                " " + words[3] + " " + words[4] + "\n")
                else:
                    file.write(line)

    # This function is part of the backend error recording. It checks that no item has a negative number of days left.
    def negative_days_check(self, words):
        if int(words[3]) < 0:
            print("ERROR: failed constraint error caused by transaction:", words[0], words[1], words[2], words[
                3], words[4], "No item should ever have a negative number of days left")
            return True
        return False
    
    # This function is part of the backend error recording. It checks that a newly created user has a name different from all existing users.
    def different_name_check(self, words):
        name = words[1]

        with open(self.curr_user_file, 'r') as file:
            for line in file:
                if line.startswith(name):
                    # do something with the line
                    print("ERROR: failed constraint error caused by transaction:", words[0], words[1], words[2], words[
                          3], words[4], "A newly created user must have a name different from all existing users")
                    return True
        return False
