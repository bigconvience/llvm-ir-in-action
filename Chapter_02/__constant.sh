#!/usr/bin/env bash

rm toy.out

clang++ constant.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out

echo -e 'build success! \n'

./toy.out
