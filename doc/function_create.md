# Define Function

标签（空格分隔）： LLVM IR

---

本文介绍如何声明和定义LLVM IR的函数。
## 声明函数
以Foo函数的声明为例：
```
int Foo(int);
```
### 创建函数签名
LLVM中使用[FunctionType::get()](https://llvm.org/doxygen/classllvm_1_1FunctionType.html#a7e89b55242c964ae61b7850e99cacef0)创建函数签名，即制定入参和出参类型.
```
FunctionType *funcType = FunctionType::get(RetTy, Params, isVarArg);
```
### 创建函数
LLVM中使用[Function::Create](https://llvm.org/doxygen/classllvm_1_1Function.html#ab7351d0170b356c9f3487f8cc8c6cafd)创建LLVM中的Function对象：
```
Function *fooFunc = Function::Create(funcType, Function::ExternalLinkage, Name, TheModule.get());
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
## 定义函数
接下来给函数Foo添加如下函数体：
```
int Foo(int left) {
    return 10;
}
```
### 创建BasicBlock
LLVM中使用[BasciBlock::Create](https://llvm.org/doxygen/classllvm_1_1BasicBlock.html#ace940beeee97c222f836fe0ac70f6cf5)来创建函数体的BasicBlock
```
 BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
```
### SetInsertPoint
使用[Builder::SetInsetPoint](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#ace45cae6925c65e9d6916e09dd5b17cc)之后，就可以插入相关指令了
```
Builder->SetInsertPoint(entry);
```

### 添加Return
使用[IRBuilderBase::CreateRet](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a265b4e9fb81978c89714d34cd5abb412)创建一个返回值：
```
 Builder->CreateRet(Builder->getInt32(10));
```
### llvm::verifyFunction
最后使用[llvm::verifyFunction](https://llvm.org/doxygen/namespacellvm.html#a26389c546573f058ad8ecbdc5c1933cf)校验函数:
```
verifyFunction(*fooFunc);
```

### 运行程序
执行命令
```
clang++ function_create.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define i32 @Foo(i32 %left) {
entry:
  ret i32 10
}
```

## 总结
### API用法
LLVM中创建函数涉及到的接口有：[FunctionType::get()](https://llvm.org/doxygen/classllvm_1_1FunctionType.html#a7e89b55242c964ae61b7850e99cacef0)创建函数签名，[Function::Create](https://llvm.org/doxygen/classllvm_1_1Function.html#ab7351d0170b356c9f3487f8cc8c6cafd)创建LLVM中的Function对象，[BasciBlock::Create](https://llvm.org/doxygen/classllvm_1_1BasicBlock.html#ace940beeee97c222f836fe0ac70f6cf5)创建函数中BasicBlock，[IRBuilderBase::CreateRet](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a265b4e9fb81978c89714d34cd5abb412)创建一个返回值，最后使用[llvm::verifyFunction](https://llvm.org/doxygen/namespacellvm.html#a26389c546573f058ad8ecbdc5c1933cf)校验函数

### 完整代码
```
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

Function *createFunc(Type *RetTy, ArrayRef<Type *> Params, std::string Name, bool isVarArg = false) {
  FunctionType *funcType = FunctionType::get(RetTy, Params, isVarArg);
  Function *fooFunc = Function::Create(funcType, Function::ExternalLinkage, Name, TheModule.get());
  return fooFunc;
}

void setFuncArgs(Function *Func, std::vector<std::string> FuncArgs) {
  unsigned Idx = 0;
  Function::arg_iterator AI, AE;
  for(AI = Func->arg_begin(), AE = Func->arg_end(); AI != AE; ++AI, ++Idx) {
      AI->setName(FuncArgs[Idx]);
    }
}

int main(int argc, char *argv[]) {
  InitializeModule();
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty()}, "Foo");
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Builder->CreateRet(Builder->getInt32(10));
  verifyFunction(*fooFunc);
  
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("left");
  setFuncArgs(fooFunc, FuncArgs);

  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_02/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_02/function_create.cpp)


