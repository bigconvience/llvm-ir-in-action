# Create Global Variables

标签（空格分隔）： 未分类

---

本文介绍如何使用LLVM创建全局变量。LLVM IR中全局变量名以符号`@`开头,如下所示：
```
@X = global i32 17
@Y = global i32 42
@Z = global [2 x i32*] [ i32* @X, i32* @Y ]
```
## 声明Int型全局变量
### 创建IRBuilder
```
// Import header
#include "llvm/IR/IRBuilder.h"

// Declare Builder
static std::unique_ptr<IRBuilder<>> Builder;

// Create a new builder for the module.
Builder = std::make_unique<IRBuilder<>>(*TheContext);
```

### 创建GlobalVariables
```
GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    return gVar;
}
```

### 设置初始值
```
  GlobalVariable *gVar = createGlob(Builder->getInt32Ty(), "variable");
  gVar->setInitializer(Builder->getInt32(21));
```
### 运行程序
执行命令
```
clang++ global_variables.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

@variable = global i32 21
```

## 总结
### API用法
[Global Variables](https://llvm.org/docs/LangRef.html#globalvars)详细介绍了LLVM IR中全局变量的用法。
[ Module::getOrInsertGlobal](https://llvm.org/doxygen/classllvm_1_1Module.html#abd8f7242df6ecb10f429c4d39403c334)创建全局变量,[Module::getGlobalVariable](https://llvm.org/doxygen/classllvm_1_1Module.html#aeb548ad4e336f7f3d3cc91578b5de0a8)获取创建的全局变量。
[ GlobalVariable::setInitializer](https://llvm.org/doxygen/classllvm_1_1GlobalVariable.html#a095f8f031d99ce3c0b25478713293dea)设置初始值。
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
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    return gVar;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  GlobalVariable *gVar = createGlob(Builder->getInt32Ty(), "variable");
  gVar->setInitializer(Builder->getInt32(21));

  TheModule->print(errs(), nullptr);
  return 0;
}
```
### 源码地址
[LLVM IR in Action/Chapter_02/](https://github.com/bigconvience/llvm-ir-in-action/tree/main/Chapter_02)
