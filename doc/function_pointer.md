# LLVM in Action:函数指针(function pointer)

标签（空格分隔）： 未分类

---
以函数指针p为例:
```
int(*p)(int, int);
```
其对应生成对应的LLVM IR
```
i32 (i32, i32)*
```
### 声明函数指针类型
先创建一个FunctionType,然后利用PointerType来创建对于国内的函数指针
```
Type *intTy = Builder->getInt32Ty();
FunctionType *funcType = FunctionType::get(intTy, {intTy, intTy}, false);
PointerType *funcPointer = PointerType::get(funcType, 0);
```
### 完整代码
```
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
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

GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    return gVar;
}


int main(int argc, char *argv[]) {
  InitializeModule();
  Type *intTy = Builder->getInt32Ty();
  FunctionType *funcType = FunctionType::get(intTy, {intTy, intTy}, false);
  PointerType *funcPointer = PointerType::get(funcType, 0);

  Function *Box3getEv = createFunc(Builder->getDoubleTy(), { intTy }, "foo");
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Box3getEv);
  Builder->SetInsertPoint(entry);
  Value *fooBar = Builder->CreateAlloca(funcPointer, nullptr, "p");

  TheModule->print(errs(), nullptr);
  return 0;
}
```
### 执行命令
```
clang++ function_pointer.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
### 输出
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

define double @foo(i32 %0) {
entry:
  %p = alloca i32 (i32, i32)*, align 8
}
```