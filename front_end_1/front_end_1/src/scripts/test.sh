# Script: test.sh
# Authors: Rija Baig, Preet Panchal, Eihab Syed, Nathaniel Tai
# Details: Iterates over all testing input files and stores outputs in
#		   a directory to be used in the comparison script.

#Set the location of the test inputs to the 'FILES' variable
# INPUT_FILES=./../testcases/inputs/*.txt;
# INPUT_FOLDERS=../testcases/inputs;
INPUT_FILES=./../testcases/inputs/*.txt;
INPUT_FOLDERS=./../testcases/inputs/*; 
TEST_FILES_LOC=./../testcases/outputs/actual;

#pre-condition files
CURRENT_USERS=../iofiles/current_users_accounts.txt
AVAIL_ITEMS=../iofiles/available_items.txt

#Color codes
GRAY='\033[0;37m';
NC='\033[0m';

#Remove previous test files
for folder in $TEST_FILES_LOC/*
do 
	for foldername in $folder/*
	do
		rm $foldername/*;
	done
done

#Clear current users and available items files
truncate -s 0 $CURRENT_USERS $AVAIL_ITEMS
i=1

#Iterate through all of the files specified in the 'FILES' variable
for folder in $INPUT_FOLDERS
do	
	printf "\n$folder\n";
	for file in $folder/*
	do
		rm $CURRENT_USERS;
		rm $AVAIL_ITEMS;
		#Fill current users file with preset users to meet pre-conditions for each test
		printf "user01 AA 100 teww\nuser02 FS 100 teww\nuser03 BS 100 teww\nuser04 SS 100 teww\n" >> $CURRENT_USERS
		printf "vicks user01 user03 10 10\nfan user01 N/A 75 75\nlotion user01 user03 5 5\nlamp user02 N/A 50 50\n" >> $AVAIL_ITEMS
		#Set the filename of the current test to be used for output
		filename=$(basename $file);
		testname=${filename%%.*};
		foldername=$(basename $folder);
		printf "\n$file\n"; 
		printf "${NC}Processing test $i: ${GRAY}$filename\n";
		i=$(($i+1))
		mkdir -p $TEST_FILES_LOC/$foldername/$testname;
		touch $TEST_FILES_LOC/$foldername/$testname/"$testname"_daily_transaction.txt
		daily=$TEST_FILES_LOC/$foldername/$testname/"$testname"_daily_transaction.txt

		#Run the auction system program and pipe output to external file named according to the current test name
		../auction_system ../iofiles/current_users_accounts.txt ../iofiles/available_items.txt $daily < $file >> $TEST_FILES_LOC/$foldername/$testname/$testname.txt
	done
done

#Run the compare script to compare test output with expected output files
./compare.sh