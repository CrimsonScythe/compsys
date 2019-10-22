#!/bin/bash

# Set path so it matches your installation:
PRERF=prerf

if [ "$#" -ne 2 ]; then
   echo "Running some x86prime programs"
   echo "Usage:"
   echo "  ./run-performance [Sorting Algorithm] [Machine]"
   echo "    Sorting Algorithm: Sorting algorithm you want to run"
   echo "    Machine: the type of machine to run"
   exit 1;
fi

algorithm=""
if [[ "${1}" == "quicksort" ]]; then
  algorithm="quicksort"
fi
if [[ "${1}" == "quicksort_2" ]]; then
  algorithm="quicksort_2"
fi
if [[ "${1}" == "heapsort" ]]; then
  algorithm="heapsort"
fi
if [[ "${1}" == "mergesort_2" ]]; then
  algorithm="mergesort_2"
fi
if [[ "${1}" == "mergesort" ]]; then
  algorithm="mergesort"
fi
if [[ "${algorithm}" == "" ]]; then
  echo "Algorithm set to unknown value: ${1}"
  exit 1;
fi

# Machine definitions
simple="-mem real -pipe simple"
superscalar="-mem real -pipe super"
ooo="-mem real -pipe ooo"

# Memory definitions
simplereal="-pipe simple -mem real"
simplemagic="-pipe simple -mem magic"

machine=""

if [[ "${2}" == "simplereal" ]]; then
  machine=${simplereal}
fi
if [[ "${2}" == "simplemagic" ]]; then
  machine=${simplemagic}
fi

if [[ "${2}" == "simple" ]]; then
  machine=${simple}
fi
if [[ "${2}" == "superscalar" ]]; then
  machine=${superscalar}
fi
if [[ "${2}" == "ooo" ]]; then
  machine=${ooo}
fi
if [[ "${machine}" == "" ]]; then
  echo "Machine set to unknown value: ${2}"
  exit 1;
fi


# Make sure code is up to date
make $1

# Create for runs
mkdir -p runs
rm -f runs/*

# Number for elements to be sorted (to be extended)
elements="100 1000 10000 20000 40000 100000"

# Generate files with inputs for things to be sorted
for elem in ${elements}
do
  printf "0\n${elem}\n" > runs/run-${elem}.input
done

echo ""
echo "Machine"
echo "-------"
echo ""
echo "Sorting with" ${1}
echo "===================================="
for elem in ${elements}
do
  datafile="data/${algorithm}-${2}-${elem}.data"
  echo ""
  echo " - sorting" ${elem} "entries"
  echo "   ------------------"
  # ${x86prime} -f ${1}.s_prime -asm $machine -run run -input runs/run-${elem}.input > ${datafile}
  ${PRERF} ${1}.hex run $machine < runs/run-${elem}.input > ${datafile}
  cat ${datafile}
done
