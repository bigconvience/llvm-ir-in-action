#!/bin/bash

rm toy.out

file_name=${0##*/}
new_extension="cpp"
# Replace .sh with .cpp
generated_file_name=${file_name/.sh/.$new_extension}
echo ${generated_file_name}

clang++ ${generated_file_name} `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out