#!/bin/bash
echo "Running part 1"
clang++ main1.cc 
./a.out
echo "Running part 2"
clang++ main2.cc -std=c++11
./a.out
