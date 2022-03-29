# LLVM in Action:Create Constant

标签（空格分隔）： LLVM IR

---

本文介绍LLVM中常量的创建。
## ConstantData
[ConstantData](https://llvm.org/doxygen/classllvm_1_1ConstantData.html)是LLVM中的常量的基类,其子类有ConstantFP,ConstantInt等，分别表示创建float-point，integer等类型。IRBuilder中也封装了相应的API，如创建一个Int 31:
```
Builder->getInt32(31);
```

## 创建全局常量
[GlobalVariable::setConstant(true)](https://llvm.org/doxygen/classllvm_1_1GlobalVariable.html#a40ae67d31da67ca17ed016839faf3390)将全局变量设置为Constant.
```
GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    gVar->setConstant(true);
    return gVar;
}

int main(int argc, char *argv[]) {
    ...
  GlobalVariable *gVar = createGlob(Builder->getInt32Ty(), "variable");
  gVar->setInitializer(Builder->getInt32(21));
    ...
}

```
生成的IR如下：
```
; ModuleID = 'first module'
source_filename = "first module"

@variable = constant i32 21
```
## 结构体常量
以struct Foo为例：
```
struct Foo {
    int a;
    float b;
}

struct Foo foo_var = {16. 3.14};
```
对应的创建代码如下
```
StructType *Foo = StructType::create(*TheContext, "Foo");
Foo->setBody({Builder->getInt32Ty(), Builder->getDoubleTy()});
GlobalVariable *gfoo = createGlob(Foo, "foo_var");
Constant *foo_constant = ConstantStruct::get(Foo, 
      {Builder->getInt32(16), ConstantFP::get(Builder->getDoubleTy(), 3.14)}); 
 gfoo->setInitializer(foo_constant);
```
生成的LLVM IR如下：
```
%Foo = type { i32, double }

@foo_var = constant %Foo { i32 16, double 3.140000e+00 }
```

### 运行程序
执行命令
```
clang++ constants.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first module'
source_filename = "first module"

%Foo = type { i32, double }

@variable = constant i32 21
@foo_var = constant %Foo { i32 16, double 3.140000e+00 }
```


## 总结
### API用法
[ConstantData](https://llvm.org/doxygen/classllvm_1_1ConstantData.html)是LLVM中的常量的基类.

[GlobalVariable::setConstant(true)](https://llvm.org/doxygen/classllvm_1_1GlobalVariable.html#a40ae67d31da67ca17ed016839faf3390)将全局变量设置为Constant.

[ConstantStruct::get](https://llvm.org/doxygen/classllvm_1_1ConstantStruct.html#a0864b95d3bc8c73f28141ac093e7eccb)创建struct constant.
### 完整代码
```
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

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

GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    gVar->setConstant(true);
    return gVar;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  GlobalVariable *gVar = createGlob(Builder->getInt32Ty(), "variable");
  gVar->setInitializer(Builder->getInt32(21));


  StructType *Foo = StructType::create(*TheContext, "Foo");
  Foo->setBody({Builder->getInt32Ty(), Builder->getDoubleTy()});
  GlobalVariable *gfoo = createGlob(Foo, "foo_var");
  Constant *foo_constant = ConstantStruct::get(Foo, 
      {Builder->getInt32(16), ConstantFP::get(Builder->getDoubleTy(), 3.14)}); 
  gfoo->setInitializer(foo_constant);

  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_02/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_02/constants.cpp)





