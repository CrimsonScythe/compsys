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
printf "\x20\x7C" > test_files/ascii3.input
printf "\x7C\x7D\x7E" > test_files/ascii4.input
printf "\x20\x7C\x7D\x7E" > test_files/ascii5.input

# Edge case testing for data file
printf "\x7\x8\x00\x9" > test_files/data.input
printf "\x6\x5" > test_files/data1.input

#Edge case testing for ISO-8859-1 file
printf "\xA0\xA1\n" > test_files/ISO.input
printf "\xFF\n" > test_files/ISO2.input
printf "\xe6\xf8\xe5\n" > test_files/ISO3.input 

# Testing for empty file 
printf "" > test_files/empty.input 

# Having ascii unless last byte is data
printf "\x20\x7C\x7D\x7E\x6" > test_files/data2.input

# Having ascii unless first byte is data
printf "\x6\x7C\x7D\x7E" > test_files/data3.input

# Having ascii unless last byte is ISO-8859-1 
printf "\x7C\x7D\x7E\xe6\n" > test_files/ISO4.input

# Having ascii unless first byte is ISO-8859-1 
printf "\xe5\x7C\x7D\x7E\n" > test_files/ISO5.input

# Having ISO-8859-1 unless first byte is data
printf "\x6\xe6\xf8\xe5\n" > test_files/data4.input 

# Having ISO-8859-1 unless last byte is ascii
printf "\xA0\xA1\x9\n" > test_files/ascii6.input

# Having ISO-8859-1 unless first byte is ascii
printf "\x9\xe6\xf8\xe5\n" > test_files/ascii7.input 

# printf "\xFF\xFE hej \n\n\n\n\n\n\n\n\n\n" > test_files/UTF-16LE.input
# printf "\xFE\xFF \n" > test_files/UTF-16BE.input

# Test file from A0
printf "Hello, World!\n" > test_files/A0ascii.input
printf "Hello, World!" > test_files/A0ascii2.input
printf "Testing\/for_ any ASCII mistake" > test_files/A0ascii3.input
printf "Der\"exist\"" > test_files/A0ascii4.input
printf ".,'........-_;:'" > test_files/A0ascii5.input
printf "                      " > test_files/A0ascii6.input
printf "NOONE\n\n\n ENOON" > test_files/A0ascii7.input
printf "NO_DANISH_LETTERS_AS_\"AE-OE AND AA\"" > test_files/A0ascii8.input
printf "123456789" > test_files/A0ascii10.input
printf "987654321" > test_files/A0ascii11.input
printf "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > test_files/A0ascii12.input
printf "Testing\/for_\x00 any ASCII mistake" > test_files/A0data1.input
printf "Der\"exist\" \x00ss" > test_files/A0data2.input
printf ".,'.......\x00.-_;:'" > test_files/A0data3.input
printf "                      " > test_files/A0data4.input
printf "NO\x00ONE\n\n\n ENOON" > test_files/A0data5.input
printf "NO_DANISH_LETTERS_AS_\"AE-\x00-OE AND AA\"" > test_files/A0data6.input
printf "\HEJSx00A" > test_files/A0data7.input
printf "1234\x0056789" > test_files/A0data8.input
printf "98765432\x00BMAN" > test_files/A0data9.input
printf "ABCDE\x00FGHIJKLMNOPQRSTUVWXYZ" > test_files/A0data10.input
printf "Hello,\x00World!\n" > test_files/A0data11.input
printf "GENERATING TESTFIL\x00ES" > test_files/A0data12.input

echo "Running the tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed 's/ASCII text.*/ASCII text/' > test_files/expected
./file  ${f} > test_files/actual
file    test.input | sed 's/ASCII text.*/ASCII text/' > test_files/noinputexp
./file  test.input > test_files/noinputact
file    wrong.input | sed 's/ASCII text.*/ASCII text/' > test_files/noinputexp
./file  wrong.input > test_files/noinputact

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

echo ">>> Testing for wrong files.."
if ! diff -u test_files/noinputexp test_files/noinputexp
then 
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Succes :-)"
fi

exit $exitcode