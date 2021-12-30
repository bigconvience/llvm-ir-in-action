# Local Vairables

标签（空格分隔）： LLVM IR

---
本文以sum10函数为例介绍如何使用LLVM局部变量.
```
int sum10(int a, int b) {
    int c = a + b;
    int d = c + 10;
    return d;
}
```
## 创建函数参数
### 设置参数名
先创建如下空函数：
```
int sum10(int a, int b) {
    return 10;
}
```
[Define Function](https://zhuanlan.zhihu.com/p/451425126)介绍了函数的定义，使用[Function::arg_iterator](https://llvm.org/doxygen/classllvm_1_1Function.html#a1c9f2f058966db1f8ca270398bda8362)设置函数参数名：
```
void setFuncArgs(Function *Func, std::vector<std::string> FuncArgs) {
  unsigned Idx = 0;
  Function::arg_iterator AI, AE;
  for(AI = Func->arg_begin(), AE = Func->arg_end(); AI != AE; ++AI, ++Idx) {
      AI->setName(FuncArgs[Idx]);
    }
}
```
### 实现函数体
设置对应的参数类型和函数名，确保一一对应：
```
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, "Foo");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Builder->CreateRet(Builder->getInt32(10));
```
### 运行程序
执行命令
```
clang++ local_variable.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define i32 @Foo(i32 %a, i32 %b) {
entry:
  ret i32 10
}
SH-jiangbenpeng%
```
## 访问局部变量
### 访问函数参数
使用[Function::arg_begin()](https://llvm.org/doxygen/classllvm_1_1Function.html#a8bf193a781a92cae52d7f9216d0824f8)来访问函数参数:
```
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;
```
### 添加运算指令
IRBuilder中封装了大量的指令创建API, 如[IRBuilderBase::CreateAdd](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a928603739e0e70713566011d44052a4f)用来加法运算：
```
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;
  Value *c = Builder->CreateAdd(Arg1, Arg2, "c");
  Value *result = Builder->CreateAdd(c, Builder->getInt32(10), "result");
  Builder->CreateRet(result);
```
### 运行程序
执行命令
```
clang++ local_variable.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define i32 @Foo(i32 %a, i32 %b) {
entry:
  %c = add i32 %a, %b
  %result = add i32 %c, 10
  ret i32 %result
}
```


## 总结
### API用法
[Function::arg_begin()](https://llvm.org/doxygen/classllvm_1_1Function.html#a8bf193a781a92cae52d7f9216d0824f8)来访问函数参数.
[Function::arg_iterator](https://llvm.org/doxygen/classllvm_1_1Function.html#a1c9f2f058966db1f8ca270398bda8362)设置函数参数名.
[IRBuilder](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html)中封装了大量LLVM指令创建API,[LLVM Language Reference Manual](https://llvm.org/docs/LangRef.html)中列出了所有LLVM指令

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
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, "Foo");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;
  Value *c = Builder->CreateAdd(Arg1, Arg2, "c");
  Value *result = Builder->CreateAdd(c, Builder->getInt32(10), "result");
  Builder->CreateRet(result);
  verifyFunction(*fooFunc);

  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_02/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_02/local_variable.cpp)





