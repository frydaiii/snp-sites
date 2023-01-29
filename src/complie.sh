#!/bin/bash

echo "Compile for AVX"
g++ -O2 -m64 -mavx -std=c++17 -c snp_sites.cpp -od7.o
echo "Compile for AVX2"
g++ -O2 -m64 -mavx2 -mfma -std=c++17 -c snp_sites.cpp -od8.o
echo "Compile for AVX512"
g++ -O2 -m64 -mavx512f -mfma -mavx512vl -mavx512bw -mavx512dq -std=c++17 -c snp_sites.cpp -od10.o
echo "The last compilation uses the lowest supported instruction set (SSE2). This includes the main program, and links all versions together:"
g++ -O2 -m64 -msse2 -std=c++17 *.cpp d7.o d8.o d10.o -lz -otest
