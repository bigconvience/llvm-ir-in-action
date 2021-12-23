#!/usr/bin/env bash

clang++ function.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out

echo -e 'build success! \n'

./toy.out