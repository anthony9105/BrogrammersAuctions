#!/bin/bash

# Define the directory paths for input, output, and expected output files
INPUT_DIR="Test_Cases/inputs"
OUTPUT_DIR="Test_Cases/outputs"
EXPECTED_DIR="Test_Cases/expected"

# Compile your C++ program
g++ main.cpp user/user.cpp transactions/transaction.cpp transactions/create.cpp transactions/delete.cpp transactions/addcredit.cpp transactions/changePassword.cpp -o main

# Loop through each input file in the input directory
for input_file in $INPUT_DIR/*
do
  # Generate the output file by running your program with the current input file
  output_file="$OUTPUT_DIR/$(basename "$input_file")_output.txt"
  timeout 0.1s ./main ../files/DailyTransactions.txt ../files/CurrentUserAccounts.txt ../files/AvailableItemsFile.txt< "$input_file" > "$output_file"
  
  # Compare the generated output file to the expected output file
  expected_file="$EXPECTED_DIR/$(basename "$input_file")_expected.txt"
  if diff -b "$output_file" "$expected_file" >/dev/null ; then
    echo "$(basename "$input_file") passed"
  else
    echo "$(basename "$input_file") failed"
  fi
done