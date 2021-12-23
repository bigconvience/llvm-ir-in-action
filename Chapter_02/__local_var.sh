#!/usr/bin/env bash

file='toy.out'
if [ -f "$file" ]; then
  rm "$file"
fi

clang++ local_var.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out

echo -e 'build success! \n'

./toy.out
