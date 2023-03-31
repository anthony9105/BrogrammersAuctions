from file_modifier import FileModifier

# The backend will first make a new file which has the new lines of the daily transaction files. It will store each line in a list for easy reading and access.
# The program then goes through this lists and makes the changes depending on the contents of the line and will use the starting number of the line to know 
# which function to call for each of the different commands that are in the frontend.
class BackendDriver:
    # empty constructor
    def __init__(self):
        pass

    # This function makes a new file which has the new lines of the daily transaction files.
    def create_daily_transaction_new_content(self):
        with open("../daily_transaction.txt", "r") as a_file:
            a_lines = set(a_file.readlines())

        with open("../merged_daily_transaction.txt", "r") as b_file, open("../daily_transaction_new_content.txt", "w") as c_file:
            for line in b_file:
                if line not in a_lines:
                    c_file.write(line + '\n')


    # This function is used to start the execution of the backend.  It reads the new daily tranaction content and applies the
    # appropriate modifications according to what is read from the daily transactions
    def execute_back_end(self):
    # FileModifier object used for the modification functions
        fm = FileModifier()

        with open("../daily_transaction_new_content.txt", "r") as c_file:
            for line in c_file:
                words = line.split()

                if len(words) < 1:  # check if there are any words in the line
                    continue

                if words[0] == '01':
                    if len(words) < 5:  # check if there are enough words to index into
                        continue
                    issue = fm.different_name_check(words)
                    if issue == False:
                        fm.create_modifications(words)

                elif words[0] == '02':
                    if len(words) < 2:
                        continue
                    fm.delete_modifications(words)

                elif words[0] == '03':
                    if len(words) < 5:
                        continue
                    issue = fm.negative_days_check(words)
                    if issue == False:
                        fm.advertise_modifications(words)

                elif words[0] == '04':  
                    if len(words) < 6:
                        continue
                    fm.bid_modifications(words)

                elif words[0] == '05':
                    if len(words) < 3:
                        continue
                    fm.refund_modifications(words)

                elif words[0] == '06':
                    if len(words) < 4:
                        continue
                    fm.addCredit_modifications(words)

                elif words[0] == '07':
                    if len(words) < 3:
                        continue
                    fm.resetPassword_modifications(words)


def main():
    bd = BackendDriver()
    bd.create_daily_transaction_new_content()
    bd.execute_back_end()

main()
