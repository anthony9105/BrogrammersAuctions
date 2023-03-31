from src.backend import BackendDriver

import pytest
from backend import BackendDriver
from file_modifier import FileModifier
from unittest.mock import MagicMock, patch

class TestBackendDriver:
    @pytest.fixture
    def backend_driver(self):
        return BackendDriver()

    @pytest.fixture
    def setup_files(self, tmpdir):
        daily_transaction = tmpdir.join("daily_transaction.txt")
        merged_daily_transaction = tmpdir.join("merged_daily_transaction.txt")
        daily_transaction_new_content = tmpdir.join("daily_transaction_new_content.txt")

        return daily_transaction, merged_daily_transaction, daily_transaction_new_content

    @patch('backend.FileModifier', autospec=True)
    def test_execute_back_end(self, MockFileModifier, backend_driver, setup_files):
        daily_transaction, merged_daily_transaction, daily_transaction_new_content = setup_files

        # Test case 2.1
        daily_transaction_new_content.write("01 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.create_modifications.called

        # Test case 2.2
        daily_transaction_new_content.write("02 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.delete_modifications.called

        # Test case 2.3
        daily_transaction_new_content.write("03 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.negative_days_check.called
        assert MockFileModifier.advertise_modifications.called

        # Test case 2.4
        daily_transaction_new_content.write("04 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.bid_modifications.called

        # Test case 2.5
        daily_transaction_new_content.write("05 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.refund_modifications.called

        # Test case 2.6
        daily_transaction_new_content.write("06 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.addCredit_modifications.called

        # Test case 2.7
        daily_transaction_new_content.write("07 ...")
        backend_driver.execute_back_end()
        assert MockFileModifier.resetPassword_modifications.called
