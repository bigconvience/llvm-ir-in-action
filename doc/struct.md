# LLVM IR in Action:Structure

标签（空格分隔）： LLVM IR Struct

---
本文介绍LLVM的[Struct](https://llvm.org/docs/LangRef.html#structure-types)的创建和使用
## 创建Struct Type
### 定义一个Struct Type
[StructType.create()](https://llvm.org/doxygen/classllvm_1_1StructType.html#ab6c57734c1613c9cb4629578d7ac611a)用来创建一个Struct Type.以struct Foo为例：
```
struct Foo
{
    int a;
    char *b;
    double c;
};
```
创建代码为：
```
StructType *Foo = StructType::create(*TheContext, "Foo");
Foo->setBody({Builder->getInt32Ty(), Builder->getInt8PtrTy(), Builder->getDoubleTy()});
```
对应生成的LLVM IR如下
```
%Foo = type { i32, i8*, double }
```
### 声明一个全局struct对象
有了struct Foo,
```
struct Foo foo_struct;
```
声明全局变量foo_struct的代码如下：
```
GlobalVariable *gVar = createGlob(Foo, "foo_struct");
```
生成的IR:
```
@foo_struct = external global %Foo
```
### 创建局部struct对象
[IRBuilderBase::CreateAlloca](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#afbe24f9e57cab99bd5bf3960c76bcc94)用来在栈中分配内存，对于如下对象：
```
Foo fooBar;
```
声明代码如下:
```
Value *fooBar = Builder->CreateAlloca(Foo, nullptr, "fooBar");
```
生成的IR:
```
 %fooBar = alloca %Foo, align 8
```
### 访问结构体成员
LLVM中访问struct Foo中的成员b,
```
char **bptr = &foo.b;
```
需要用[IRBuilderBase::CreateGEP](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a4e4be43def2aaea99a194dda41979444)+成员的索引来获取，对应代码如下：
```
Value *b = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(1)}, "b");
```
生成的IR:
```
%b = getelementptr %Foo, %Foo* %fooBar, i32 0, i32 1
```
### 结构体成员赋值
若要给struct Foo中的成员c赋值，
```
foo.c = 3.14;
```
先要通过[IRBuilderBase::CreateGEP](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a4e4be43def2aaea99a194dda41979444)来获取c的地址，然后通过[IRBuilderBase::CreateStore](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a7d1a18dc150da601587ccc716e0e4b41)给成员c赋值：
```
Value *c = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(2)}, "c");
Builder->CreateStore(ConstantFP::get(Builder->getDoubleTy(), 3.14), c);
```
生成的IR:
```
%c = getelementptr %Foo, %Foo* %fooBar, i32 0, i32 2
store double 3.140000e+00, double* %c, align 8
```


## 访问结构体成员
### 取成员数据
LLVM中使用[FunctionType::get()](https://llvm.org/doxygen/classllvm_1_1FunctionType.html#a7e89b55242c964ae61b7850e99cacef0)创建函数签名，即制定入参和出参类型.
```
FunctionType *funcType = FunctionType::get(RetTy, Params, isVarArg);
```

### 运行程序
执行命令
```
clang++ struct.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first module'
source_filename = "first module"

%Foo = type { i32, i8*, double }

@foo_struct = external global %Foo

define %Foo* @Bar(i32 %a) {
entry:
  %result = add i32 %a, 10
  %fooBar = alloca %Foo, align 8
  %b = getelementptr %Foo, %Foo* %fooBar, i32 0, i32 1
  %c = getelementptr %Foo, %Foo* %fooBar, i32 0, i32 2
  store double 3.140000e+00, double* %c, align 8
  ret %Foo* %fooBar
}
```

## 总结
### API用法
[StructType.create()](https://llvm.org/doxygen/classllvm_1_1StructType.html#ab6c57734c1613c9cb4629578d7ac611a)用来创建一个Struct Type.

[IRBuilderBase::CreateAlloca](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#afbe24f9e57cab99bd5bf3960c76bcc94)用来在栈中分配内存.

Struct中成员访问，先通过[IRBuilderBase::CreateGEP](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a4e4be43def2aaea99a194dda41979444)来获取c的地址，然后通过[IRBuilderBase::CreateStore](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a7d1a18dc150da601587ccc716e0e4b41)给成员c赋值.

[PointerType::get](https://llvm.org/doxygen/classllvm_1_1PointerType.html#a050a395d1656e546679d8b144d951bb2)用来创建指针对象》

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
  TheModule = std::make_unique<Module>("first module", *TheContext);
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

GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    return gVar;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  StructType *Foo = StructType::create(*TheContext, "Foo");
  Foo->setBody({Builder->getInt32Ty(), Builder->getInt8PtrTy(), Builder->getDoubleTy()});

  GlobalVariable *gVar = createGlob(Foo, "foo_struct");

  Function *fooFunc = createFunc(PointerType::get(Foo, 0), {Builder->getInt32Ty()}, "Bar");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *result = Builder->CreateAdd(Arg1, Builder->getInt32(10), "result");

  Value *fooBar = Builder->CreateAlloca(Foo, nullptr, "fooBar");
  Value *b = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(1)}, "b");

  Value *c = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(2)}, "c");
  Builder->CreateStore(ConstantFP::get(Builder->getDoubleTy(), 3.14), c);

  Builder->CreateRet(fooBar);
  verifyFunction(*fooFunc);

  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_02/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_02/struct.cpp)





