# Script: compare.sh
# Authors: Rija Baig, Preet Panchal, Eihab Syed, Nathaniel Tai
# Details: Compares the output provided by the test.sh script with
#		   the predetermined expected output files. Returns PASS or
#		   FAIL depending on differences.

#DIRECTORIES
ACTUAL_OUTPUTS=./../testcases/outputs/actual
EXPECTED_OUTPUTS=./../testcases/outputs/expected
DIFFERENCES=./../testcases/outputs/differences

#COLOR CODES
NC='\033[0m';
GRAY='\033[0;37m'
FAIL='\033[0;31m';
PASS='\033[0;32m';

#Remove previous DIFFERENCES files
for folder in $DIFFERENCES/*
do 
	for diff_folder in $folder/*
	do
		rm $diff_folder/*;
	done
done

#Iterate over all actual output files
for act_folder in $ACTUAL_OUTPUTS/*
do
	for test_folder in $act_folder/*
	do
		for actual_file in $test_folder/*
		do
		#Iterate over all expected output files
			for exp_folder in $EXPECTED_OUTPUTS/*
			do
				for exp_test_folder in $exp_folder/*
				do
					for expected_file in $exp_test_folder/*
					do 
						#Store filenames and test names
						base_actual=$(basename $actual_file)
						base_expected=$(basename $expected_file)
						testname=${base_actual%%.*}
		
						#If actual output file name matches expected output file name
						if [ "${base_actual%%.*}" = "${base_expected%%.*}" ]
						then
							printf "\n${NC}Comparing ${GRAY}$testname";
							foldername=$(basename $exp_folder);
							mkdir -p $DIFFERENCES/$foldername/$test_folder;
							
							#Compare difference between actual output and expected output and place into differences directory
							diff -u $expected_file $actual_file > $DIFFERENCES/$foldername/$test_folder/$testname.txt
							
							#Check if diff command returned true (failed) or false (passed)
							if [ $? -ne 0 ]; then
								printf " --- ${FAIL}FAILED";
							else
								printf " --- ${PASS}PASSED";
							fi
						fi
					done
				done
			done
		done
	done
done