# LLVM in Action:if-then-else phi版本

标签（空格分隔）： LLVM

---

本文介绍如何使用LLVM中的[phi-instructio](https://llvm.org/docs/LangRef.html#phi-instruction)生成if-then-else IR.
## 创建control flow
以下为计算两个Integer的最大值函数max:
```
int max(int a, int b) {
  if (a > b) {
    return a + 1;
  } else {
    return b * 16;
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
entry中的指令直接根据a和b的大小来决定跳转到哪个分支：
```
// entry
Builder->SetInsertPoint(entry);
// if condition
Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");
Builder->CreateCondBr(Cond, ThenBB, ElseBB);
```
生成的IR:
```
entry:
  %cmptmp = icmp ult i32 %a, %b
  %ifcond = icmp ne i1 %cmptmp, false
  br i1 %ifcond, label %then, label %else
```

**then和else中的指令**
then BasicBlock将*a + 1*的值传给thenVal, else BasicBlock中将*b * 16*的值传给elseVal.then和else BasicBlock最后都跳转到ifcond中：
```
// Then 
Builder->SetInsertPoint(ThenBB);
Value *ThenVal = Builder->CreateAdd(Arg1, Builder->getInt32(1), "thenVal");
Builder->CreateBr(MergeBB);

// else
Builder->SetInsertPoint(ElseBB);
Value *ElseVal = Builder->CreateMul(Arg2, Builder->getInt32(16), "elseVal");
Builder->CreateBr(MergeBB);
```
生成IR如下
```
then:                                             ; preds = %entry
  %thenVal = add i32 %a, 1
  br label %ifcont

else:                                             ; preds = %entry
  %elseVal = mul i32 %b, 16
  br label %ifcont
```
**ifconf中的指令**
ifconf BasicBlock中创建phi作为函数返回值：
```
// end
Builder->SetInsertPoint(MergeBB);
PHINode *Phi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "iftmp");
Phi->addIncoming(ThenVal, ThenBB);
Phi->addIncoming(ElseVal, ElseBB);
```
生成IR:
```
ifcont:                                           ; preds = %else, %then
  %iftmp = phi i32 [ %thenVal, %then ], [ %elseVal, %else ]
  ret i32 %iftmp
```

### 运行程序
执行命令
```
clang++ ifelse_phi.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define i32 @max(i32 %a, i32 %b) {
entry:
  %cmptmp = icmp ult i32 %a, %b
  %ifcond = icmp ne i1 %cmptmp, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  %thenVal = add i32 %a, 1
  br label %ifcont

else:                                             ; preds = %entry
  %elseVal = mul i32 %b, 16
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %iftmp = phi i32 [ %thenVal, %then ], [ %elseVal, %else ]
  ret i32 %iftmp
}
```

## 总结
### API用法
[IRBuilderBase::CreatePHI](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#aaba03126d9b8603419e88d63773841b1)创建PHINode

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
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
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


void createMax_phi() {
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
  // if condition
  Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
  Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");
  Builder->CreateCondBr(Cond, ThenBB, ElseBB);

  // Then 
  Builder->SetInsertPoint(ThenBB);
  Value *ThenVal = Builder->CreateAdd(Arg1, Builder->getInt32(1), "thenVal");
  Builder->CreateBr(MergeBB);

  // else
  Builder->SetInsertPoint(ElseBB);
  Value *ElseVal = Builder->CreateMul(Arg2, Builder->getInt32(16), "elseVal");
  Builder->CreateBr(MergeBB);

  // end
  Builder->SetInsertPoint(MergeBB);
  PHINode *Phi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "iftmp");
  Phi->addIncoming(ThenVal, ThenBB);
  Phi->addIncoming(ElseVal, ElseBB);

  Builder->CreateRet(Phi);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  createMax_phi();

  TheModule->print(outs(), nullptr);
  return 0;
}

```

### 源码地址

[LLVM IR in Action/Chapter_03/ifelse_phi.cpp](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_03/ifelse_phi.cpp)






