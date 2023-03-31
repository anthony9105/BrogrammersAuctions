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

        # Configure the return_value of the FileModifier constructor to have the create_modifications method
        mock_instance = MockFileModifier.return_value
        mock_instance.create_modifications = MagicMock()

        # Patch the BackendDriver to use the mocked instance of FileModifier
        backend_driver.file_modifier = mock_instance

        # Test case 2.1
        with open(daily_transaction_new_content, "w") as f:
            f.write("01 ...\n")
        
        # Update the daily_transaction file with the new content
        with open(daily_transaction, "w") as f:
            f.write("01 ...\n")

        backend_driver.execute_back_end()
        assert mock_instance.create_modifications.called

        # Test case 2.2
        with open(daily_transaction_new_content, "w") as f:
            f.write("02 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.delete_modifications.called

        # Test case 2.3
        with open(daily_transaction_new_content, "w") as f:
            f.write("03 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.negative_days_check.called
        assert MockFileModifier.return_value.advertise_modifications.called

        # Test case 2.4
        with open(daily_transaction_new_content, "w") as f:
            f.write("04 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.bid_modifications.called
        
        # Test case 2.5
        with open(daily_transaction_new_content, "w") as f:
            f.write("05 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.refund_modifications.called

        # Test case 2.6
        with open(daily_transaction_new_content, "w") as f:
            f.write("06 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.addCredit_modifications.called
        
            # Test case 2.7
        with open(daily_transaction_new_content, "w") as f:
            f.write("07 ...\n")
        backend_driver.execute_back_end()
        assert MockFileModifier.return_value.resetPassword_modifications.called
