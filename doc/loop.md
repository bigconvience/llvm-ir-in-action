# LLVM in Action:create loop

标签（空格分隔）： 未分类

---

本文介绍如何使用LLVM生成if-then-else IR.
## 创建control flow
以对a和b进行连加计算的函数sum为例:
```
int sum(int a, int b) {
  int addTmp = a - b;
  for (int i = a; i <= b; i++) {
    addTmp = addTmp + i;
  }
  return addTmp;
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
定义了函数sum：
```
declare i32 @sum(i32 %a, i32 %b);
```
### 设置函数参数
创建并设置函数参数：
```
Function *fooFunc = createMaxProto("sum");
// args
Function::arg_iterator AI = fooFunc->arg_begin();
Value *StartVal = AI++;
Value *EndVal = AI;
```
### 创建BasicBlock
在LLVM IR中，需要创建四个BasicBlock:entryBB,loopBB,endEntryBB,endLoopBB来实现loop:
```
  BasicBlock *entryBB = createBB(fooFunc, "entry");
  BasicBlock *loopBB = createBB(fooFunc, "loop");
  BasicBlock *endEntryBB = createBB(fooFunc, "endEntry");
  BasicBlock *endLoopBB = createBB(fooFunc, "endLoop");

```
**entry中的指令**
entry中的指令先设置初始值给initVal, 然后根据a和b的大小决定是否进入loop:
```
// entry
Builder->SetInsertPoint(entryBB);
Value *initVal = Builder->CreateSub(StartVal, EndVal, "init");
Value *EndCond = Builder->CreateICmpULE(StartVal, EndVal, "entryEndCond");
EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "entryCond");
Builder->CreateCondBr(EndCond, loopBB, endEntryBB);
```
生成的IR:
```
entry:
  %init = sub i32 %a, %b
  %entryEndCond = icmp ule i32 %a, %b
  %entryCond = icmp ne i1 %entryEndCond, false
  br i1 %entryCond, label %loop, label %endEntry
```

**loop中的指令**
loop中的指令先为i和sum创建PHINode, 然后i自增1，sum自增i,最后判断i<=b来决定是否跳出loop:
```
Builder->SetInsertPoint(loopBB);

PHINode *iPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "i");
iPhi->addIncoming(StartVal, entryBB);
PHINode *sumPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "sum");

sumPhi->addIncoming(initVal, loopBB);
Value *nextI = Builder->CreateAdd(iPhi, Builder->getInt32(1), "nextI");
Value *nextSum = Builder->CreateAdd(sumPhi, iPhi, "nextSum");

EndCond = Builder->CreateICmpULE(nextI, EndVal, "loopEndCond");
EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "loopCond");
Builder->CreateCondBr(EndCond, loopBB, endLoopBB);

iPhi->addIncoming(nextI, loopBB);
sumPhi->addIncoming(nextSum, loopBB);
```
生成IR如下
```
loop:                                             ; preds = %loop, %entry
  %i = phi i32 [ %a, %entry ], [ %nextI, %loop ]
  %sum = phi i32 [ %init, %loop ], [ %nextSum, %loop ]
  %nextI = add i32 %i, 1
  %nextSum = add i32 %sum, %i
  %loopEndCond = icmp ule i32 %nextI, %b
  %loopCond = icmp ne i1 %loopEndCond, false
  br i1 %loopCond, label %loop, label %endLoop
```
**endLoop和endInit中的指令**
endLoop返回sum，endInit返回initVal:
```
// endLoopBB
Builder->SetInsertPoint(endLoopBB);
Builder->CreateRet(sumPhi);

// endInit
Builder->SetInsertPoint(endEntryBB);
Builder->CreateRet(initVal);
```
生成IR:
```
endEntry:                                         ; preds = %entry
  ret i32 %init

endLoop:                                          ; preds = %loop
  ret i32 %sum
```

### 运行程序
执行命令
```
clang++ loop.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define i32 @sum(i32 %a, i32 %b) {
entry:
  %init = sub i32 %a, %b
  %entryEndCond = icmp ule i32 %a, %b
  %entryCond = icmp ne i1 %entryEndCond, false
  br i1 %entryCond, label %loop, label %endEntry

loop:                                             ; preds = %loop, %entry
  %i = phi i32 [ %a, %entry ], [ %nextI, %loop ]
  %sum = phi i32 [ %init, %loop ], [ %nextSum, %loop ]
  %nextI = add i32 %i, 1
  %nextSum = add i32 %sum, %i
  %loopEndCond = icmp ule i32 %nextI, %b
  %loopCond = icmp ne i1 %loopEndCond, false
  br i1 %loopCond, label %loop, label %endLoop

endEntry:                                         ; preds = %entry
  ret i32 %init

endLoop:                                          ; preds = %loop
  ret i32 %sum
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

void createSum() {
  Function *fooFunc = createMaxProto("sum");

  // args
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *StartVal = AI++;
  Value *EndVal = AI;

  BasicBlock *entryBB = createBB(fooFunc, "entry");
  BasicBlock *loopBB = createBB(fooFunc, "loop");
  BasicBlock *endEntryBB = createBB(fooFunc, "endEntry");
  BasicBlock *endLoopBB = createBB(fooFunc, "endLoop");

  // entry
  Builder->SetInsertPoint(entryBB);
  Value *initVal = Builder->CreateSub(StartVal, EndVal, "init");
  Value *EndCond = Builder->CreateICmpULE(StartVal, EndVal, "entryEndCond");
  EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "entryCond");
  Builder->CreateCondBr(EndCond, loopBB, endEntryBB);

  // loop 
  Builder->SetInsertPoint(loopBB);

  PHINode *iPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "i");
  iPhi->addIncoming(StartVal, entryBB);
  PHINode *sumPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "sum");
  
  sumPhi->addIncoming(initVal, loopBB);
  Value *nextI = Builder->CreateAdd(iPhi, Builder->getInt32(1), "nextI");
  Value *nextSum = Builder->CreateAdd(sumPhi, iPhi, "nextSum");

  EndCond = Builder->CreateICmpULE(nextI, EndVal, "loopEndCond");
  EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "loopCond");
  Builder->CreateCondBr(EndCond, loopBB, endLoopBB);

  iPhi->addIncoming(nextI, loopBB);
  sumPhi->addIncoming(nextSum, loopBB);

  // endLoopBB
  Builder->SetInsertPoint(endLoopBB);
  Builder->CreateRet(sumPhi);

  // endInit
  Builder->SetInsertPoint(endEntryBB);
  Builder->CreateRet(initVal);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  createSum();

  TheModule->print(outs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_03/loop.cpp](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_03/loop.cpp)






