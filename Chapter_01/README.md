用到的命令

clang hello.c -o a.out
clang -emit-llvm -S hello.c -o hello.ll

clang++ hello.cpp -o a.out
clang++ -emit-llvm -S hello.cpp -o hello.ll

llvm-as hello.ll -o hello.bc

llvm-dis hello.bc -o hello.ll

llc hello.ll -o hello.s

lli hello.ll
