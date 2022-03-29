# LLVM in Action:if-then-else alloca版本

标签（空格分隔）： LLVM

---
本文介绍如何使用LLVM生成if-then-else IR.
## 创建control flow
以下为计算两个Integer的最大值函数max:
```
int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
```
### 创建函数原型
```
Function *createMaxProto(std::string funcName) {
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, funcName);
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);
  return fooFunc;
}
```
定义了函数max：
```
declare i32 @max(i32 %a, i32 %b);
```

### 创建BasicBlock
在LLVM IR中，需要创建四个BasicBlock:entry,then,else,exit来实现if-then-else:
```
BasicBlock *entry = createBB(fooFunc, "entry");
BasicBlock *ThenBB = createBB(fooFunc, "then");
BasicBlock *ElseBB = createBB(fooFunc, "else");
BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
```
**entry中的指令**
entry中的指令先在栈上分配返回值内存，然后根据业务逻辑来决定跳转到哪个分支：
```
// entry
Builder->SetInsertPoint(entry);
Value *retVal = Builder->CreateAlloca(Builder->getInt32Ty(), nullptr, "retVal");

// if condition
Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");
Builder->CreateCondBr(Cond, ThenBB, ElseBB);
```
生成的IR:
```
entry:
  %retVal = alloca i32, align 4
  %cmptmp = icmp ult i32 %a, %b
  %ifcond = icmp ne i1 %cmptmp, false
  br i1 %ifcond, label %then, label %else
```

**then和else中的指令**
then BasicBlock将a的值传给retVal, else BasicBlock中将b的值传给retVal.then和else BasicBlock最后都跳转到ifcond中：
```
// Then 
Builder->SetInsertPoint(ThenBB);
Builder->CreateStore(Arg1, retVal);
Builder->CreateBr(MergeBB);

// else
Builder->SetInsertPoint(ElseBB);
Builder->CreateStore(Arg2, retVal);
Builder->CreateBr(MergeBB);
```
生成IR如下
```
then:                                             ; preds = %entry
  store i32 %a, i32* %retVal, align 4
  br label %ifcont

else:                                             ; preds = %entry
  store i32 %b, i32* %retVal, align 4
  br label %ifcont
```
**ifconf中的指令**
ifconf BasicBlock中load retVal的值作为函数返回值：
```
// end
Builder->SetInsertPoint(MergeBB);
Value *maxVal = Builder->CreateLoad(Builder->getInt32Ty(), retVal);
Builder->CreateRet(maxVal);
```
生成IR:
```
ifcont:                                           ; preds = %else, %then
  %0 = load i32, i32* %retVal, align 4
  ret i32 %0
```

### 运行程序
执行命令
```
clang++ ifelse.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first module'
source_filename = "first module"

define i32 @max(i32 %a, i32 %b) {
entry:
  %retVal = alloca i32, align 4
  %cmptmp = icmp ult i32 %a, %b
  %ifcond = icmp ne i1 %cmptmp, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  store i32 %a, i32* %retVal, align 4
  br label %ifcont

else:                                             ; preds = %entry
  store i32 %b, i32* %retVal, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %0 = load i32, i32* %retVal, align 4
  ret i32 %0
}
```


## 总结
### API用法
[IRBuilderBase::CreateBr](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a9d4ffac19ab652be9d311a0d91e1c319) 无条件跳转

[IRBuilderBase::CreateCondBr](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a0c37a4d16fc01f4ccde0fdb22df9eeac)根据条件跳转分支

### 完整代码
```
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first module", *TheContext);
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

BasicBlock *createBB(Function *fooFunc, std::string Name) {
  return BasicBlock::Create(*TheContext, Name, fooFunc);
}

Function *createMaxProto(std::string funcName) {
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, funcName);
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);
  return fooFunc;
}

void createMax() {
  Function *fooFunc = createMaxProto("max");
  // args
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;

  BasicBlock *entry = createBB(fooFunc, "entry");
  BasicBlock *ThenBB = createBB(fooFunc, "then");
  BasicBlock *ElseBB = createBB(fooFunc, "else");
  BasicBlock *MergeBB = createBB(fooFunc, "ifcont");

  // entry
  Builder->SetInsertPoint(entry);
  Value *retVal = Builder->CreateAlloca(Builder->getInt32Ty(), nullptr, "retVal");

  // if condition
  Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
  Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");
  Builder->CreateCondBr(Cond, ThenBB, ElseBB);

  // Then 
  Builder->SetInsertPoint(ThenBB);
  Builder->CreateStore(Arg1, retVal);
  Builder->CreateBr(MergeBB);

  // else
  Builder->SetInsertPoint(ElseBB);
  Builder->CreateStore(Arg2, retVal);
  Builder->CreateBr(MergeBB);

  // end
  Builder->SetInsertPoint(MergeBB);
  Value *maxVal = Builder->CreateLoad(Builder->getInt32Ty(), retVal);
  Builder->CreateRet(maxVal);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  createMax();

  TheModule->print(outs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_03/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_03/ifelse.cpp)






