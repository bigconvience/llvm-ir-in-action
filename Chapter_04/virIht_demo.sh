#!/bin/bash

file_name=${0##*/}
new_extension="cpp"
# Replace .sh with .cpp
generated_file_name=${file_name/.sh/.$new_extension}

clang++ -emit-llvm -S ${generated_file_name}
