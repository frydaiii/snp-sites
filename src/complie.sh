#!/bin/bash

# compile for AVX
g++ -m64 -mavx -std=c++17 -c snp_sites.cpp -lz -od7.o
# Compile for AVX2
g++ -O2 -m64 -mavx2 -mfma -std=c++17 -c snp_sites.cpp -lz -od8.o
# Compile for AVX512
g++ -O2 -m64 -mavx512f -mfma -mavx512vl -mavx512bw -mavx512dq -std=c++17 -c snp_sites.cpp -lz -od10.o
# The last compilation uses the lowest supported instruction set (SSE2)
# This includes the main program, and links all versions together:
g++ -O2 -m64 -msse2 -std=c++17 -I ./vectorclass *.cpp vectorclass/instrset_detect.cpp d7.o d8.o d10.o -lz -otest
