#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
# Edge case testing for ASCII file
printf "\x7\x8\x9" > test_files/ascii.input
printf "11\12\13" > test_files/ascii1.input
printf "\x7\x8\x9\10\11\12\13" > test_files/ascii2.input
printf "\x" > test_files/ascii3.input
printf "\x7C\x7D\x7E" > test_files/ascii4.input
printf "\x\x7C\x7D\x7E" > test_files/ascii5.input

# Edge case testing for data file
printf "\x7\x8\x00\x9" > test_files/data.input
printf "\x6\x5" > test_files/data1.input

#Edge case testing for ISO-8859-1 file
printf "\xA0\xA1\n" > test_files/ISO.input
printf "\xFF\n" > test_files/ISO2.input
printf "\xe6\xf8\xe5\n" > test_files/ISO3.input 

# Testing for empty file 
printf "" > test_files/empty.input 
printf "\x0048 \xe3\n" > test_files/empty2.input 





# printf "\xFF\xFE \n\0a " > test_files/UTF-16LE.input
# printf "\xFE\xFF \n" > test_files/UTF-16BE.input


### TODO: Generate more test files ###

echo "Running the tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed 's/ASCII text.*/ASCII text/' > test_files/expected
./file  ${f} > test_files/actual

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode