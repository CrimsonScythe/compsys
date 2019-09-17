#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Det er i dag et hvert et solskin vejr!" > test_files/ascii3.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "" > test_files/empty.input
printf "ÆØÅ\n" > test_files/iso.input
iconv -t ISO_8859-15 test_files/iso.input > test_files/iso.input 


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
