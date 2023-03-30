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
                    c_file.write(line)


    # This function is used to start the execution of the backend.  It reads the new daily tranaction content and applies the
    # appropriate modifications according to what is read from the daily transactions
    def execute_back_end(self):
        # FileModifier object used for the modification functions
        fm = FileModifier()

        with open("../daily_transaction_new_content.txt", "r") as c_file:
            for line in c_file:
                words = line.split()

                if words[0] == '01':
                    issue = fm.different_name_check(words)
                    if issue == False:
                        fm.create_modifications(words)

                elif words[0] == '02':
                    fm.delete_modifications(words)

                elif words[0] == '03':
                    issue = fm.negative_days_check(words)
                    if issue == False:
                        fm.advertise_modifications(words)

                elif words[0] == '04':  
                    fm.bid_modifications(words)

                elif words[0] == '05':
                    fm.refund_modifications(words)

                elif words[0] == '06':
                    fm.addCredit_modifications(words)

                elif words[0] == '07':
                    fm.resetPassword_modifications(words)

def main():
    bd = BackendDriver()
    bd.create_daily_transaction_new_content()
    bd.execute_back_end()

main()