import pytest
from file_modifier import FileModifier

class TestFileModifier:
    @pytest.fixture
    def file_modifier(self):
        return FileModifier()

    @pytest.fixture
    def setup_files(self, tmpdir):
        curr_user_file = tmpdir.join("current_users_accounts.txt")
        avail_items_file = tmpdir.join("available_items.txt")

        return curr_user_file, avail_items_file

    def test_create_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        words = ["01", "user01", "AA", "100", "password"]
        file_modifier.create_modifications(words)

        with open(curr_user_file, "r") as f:
            content = f.read()

        expected = (words[1] + " " + words[2] +
                    " " + words[3] + " " + words[4] + "\n")
        assert content == expected

    def test_delete_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        words = ["02", "user01", "AA", "100", "password"]
        curr_user_file.write("user01 AA 100 password\n")
        file_modifier.delete_modifications(words)

        with open(curr_user_file, "r") as f:
            content = f.read()
        expected = ""
        assert content == expected

    def test_advertise_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.avail_items_file = avail_items_file.strpath

        words = ["03", "item01", "user01", "3", "500"]
        file_modifier.advertise_modifications(words)

        with open(avail_items_file, "r") as f:
            content = f.read()
        expected = "item01 user01 N/A 3 500\n"
        assert content == expected
        
    def test_bid_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath
        file_modifier.avail_items_file = avail_items_file.strpath

        words = ["04", "item01", "user01", "100", "2"]
        avail_items_file.write("item01 user01 user02 100 3\n")
        curr_user_file.write("user01 AA 1000 password\n")

        file_modifier.bid_modifications(words)

        with open(avail_items_file, "r") as f:
            content = f.read()
        expected = "item01 user01 user01 100 2\n"
        assert content == expected

        with open(curr_user_file, "r") as f:
            content = f.read()
        expected = "user01 AA 998 password\n"
        assert content == expected

    def test_refund_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        words = ["05", "user01", "user02", "3", "0"]
        curr_user_file.write("user01 AA 1000 password\nuser02 BB 1000 password\n")

        file_modifier.refund_modifications(words)

        with open(curr_user_file, "r") as f:
            content = f.read()
        expected = "user01 AA 1003 password\nuser02 BB 997 password\n"
        assert content == expected

    def test_addCredit_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        words = ["06", "user01", "AA", "1000", "100"]
        curr_user_file.write("user01 AA 1000 password\n")

        file_modifier.addCredit_modifications(words)

        with open(curr_user_file, "r") as f:
            content = f.read()
        expected = "user01 AA 1000 100\n"
        assert content == expected

    def test_resetPassword_modifications(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        words = ["07", "user01", "AA", "1000", "new_password"]
        curr_user_file.write("user01 AA 1000 password\n")

        file_modifier.resetPassword_modifications(words)

        with open(curr_user_file, "r") as f:
            content = f.read()
        expected = "user01 AA 1000 new_password\n"
        assert content == expected

    def test_negative_days_check(self, file_modifier):
        words = ["03", "item01", "user01", "-3", "500"]
        assert file_modifier.negative_days_check(words) is True

        words = ["03", "item01", "user01", "3", "500"]
        assert file_modifier.negative_days_check(words) is False

    def test_different_name_check(self, file_modifier, setup_files):
        curr_user_file, avail_items_file = setup_files
        file_modifier.curr_user_file = curr_user_file.strpath

        curr_user_file.write("user01 AA 1000 password\n")

        words = ["01", "user01", "AA", "1000", "password"]
        assert file_modifier.different_name_check(words) is True

        words = ["01", "user02", "BB", "1000", "password"]
        assert file_modifier.different_name_check(words) is False



