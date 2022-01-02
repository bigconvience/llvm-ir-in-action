Description
## T1
以Foo函数的声明为例：
```
int Foo(int);
```
### Sub1
LLVM中使用[FunctionType::get()](https://llvm.org/doxygen/classllvm_1_1FunctionType.html#a7e89b55242c964ae61b7850e99cacef0)创建函数签名，即制定入参和出参类型.
```
FunctionType *funcType = FunctionType::get(RetTy, Params, isVarArg);
```

### 运行程序
执行命令
```
clang++ function_create.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
declare i32 @Foo(i32)
```


## 总结
### API用法


### 完整代码
```

```

### 源码地址

[LLVM IR in Action/Chapter_03/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_03/ifelse.cpp)


