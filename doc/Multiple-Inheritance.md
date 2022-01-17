# LLVM in Action:多继承(Multiple Inheritance)

标签（空格分隔）： 未分类

---

本文介绍如何使用LLVM实现多继承
## 实现多继承
定义C++类Box, Square和Cube, Cube继承Box和Square:
```
class Box
{
public:
    void SetA(double value)
    {
        _a = value;
    }

private:
    double _a;
};

class Square
{
public:
    void SetB(double value) {
        _b = value;
    }

private:
    double _b;
};

class Cube:public Box, public Square {
public:
    void SetC(double value) {
        SetA(value);
        SetB(value);
        _c = value;
    }
private:
    int _c;
};
```
### 创建Box
Box对应的struct定义与函数生成代码如下
```
StructType *createBoxTy() {
  // create Box class struct
  StructType *Box = StructType::create(*TheContext, "class.Box");
  Box->setBody(Builder->getDoubleTy());

  // create void Box::SetA(double value)
  Function *Box3getEv = createFunc(Builder->getVoidTy(), { PointerType::get(Box, 0), Builder->getDoubleTy()}, "Box_SetA");
  std::vector<std::string> BoxGetFuncArgs;
  BoxGetFuncArgs.push_back("this");
  BoxGetFuncArgs.push_back("value");
  setFuncArgs(Box3getEv, BoxGetFuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Box3getEv);
  Builder->SetInsertPoint(entry);

  Function::arg_iterator getAI = Box3getEv->arg_begin();
  Value *getThis = getAI++;
  Value *value = getAI++;
  setMemberValue(Box, getThis, 0, "_a", value);
  verifyFunction(*Box3getEv);
  return Box;
}

```
最终生成的IR为：
```
%class.Box = type { double }
define void @Box_SetA(%class.Box* %this, double %value) {
entry:
  %_a = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
  store double %value, double* %_a, align 8
}
```

### 创建Square
Square对应的struct定义与函数生成代码如下
```
StructType *createSquareTy() {
  // Square class struct
  StructType *Square = StructType::create(*TheContext, "class.Square");
  Square->setBody(Builder->getDoubleTy());

  // create void Square::Square_SetB(double value)
  Function *Square_SetB = createFunc(Builder->getVoidTy(), { PointerType::get(Square, 0), Builder->getDoubleTy()}, "Square_SetB");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  FuncArgs.push_back("value");
  setFuncArgs(Square_SetB, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Square_SetB);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = Square_SetB->arg_begin();
  Value *This = getAI++;
  Value *value = getAI++;
  setMemberValue(Square, This, 0, "_b", value);
  verifyFunction(*Square_SetB);
  return Square;
}
```
最终生成的IR为：
```
%class.Square = type { double }

define void @Square_SetB(%class.Square* %this, double %value) {
entry:
  %_b = getelementptr %class.Square, %class.Square* %this, i32 0, i32 0
  store double %value, double* %_b, align 8
}
```
### 创建Cube
Cube对应的struct定义和函数生成的代码为：
```
StructType *createCubeTy(StructType *BoxTy, StructType *SquareTy) {
  // Cube class struct
  StructType *Cube = StructType::create(*TheContext, "class.Cube");
  Cube->setBody(BoxTy, SquareTy, Builder->getDoubleTy());

  // create void Cube::Cube_SetC(double value)
  Function *Cube_SetC = createFunc(Builder->getVoidTy(), { PointerType::get(Cube, 0), Builder->getDoubleTy()}, "Cube_SetC");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  FuncArgs.push_back("value");
  setFuncArgs(Cube_SetC, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Cube_SetC);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = Cube_SetC->arg_begin();
  Value *This = getAI++;
  Value *value = getAI++;

  Value *boxObj = Builder->CreateBitCast(This, PointerType::get(BoxTy, 0), "box");
  Function *Box_SetA = TheModule->getFunction("Box_SetA");
  Builder->CreateCall(Box_SetA, {boxObj, value});

  Value *squareObj = Builder->CreateBitCast(This, PointerType::get(SquareTy, 0), "square");
  Function *Square_SetB = TheModule->getFunction("Square_SetB");
  Builder->CreateCall(Square_SetB, {squareObj, value});

  setMemberValue(Cube, This, 2, "_c", value);
  verifyFunction(*Cube_SetC);
  return Cube;
}
```
最终生成的IR为：
```
%class.Cube = type { %class.Box, %class.Square, double }

define void @Cube_SetC(%class.Cube* %this, double %value) {
entry:
  %box = bitcast %class.Cube* %this to %class.Box*
  call void @Box_SetA(%class.Box* %box, double %value)
  %square = bitcast %class.Cube* %this to %class.Square*
  call void @Square_SetB(%class.Square* %square, double %value)
  %_c = getelementptr %class.Cube, %class.Cube* %this, i32 0, i32 2
  store double %value, double* %_c, align 8
}
```

### 运行程序
执行命令
```
clang++ multiple_inheritance.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"

%class.Box = type { double }
%class.Square = type { double }
%class.Cube = type { %class.Box, %class.Square, double }

define void @Box_SetA(%class.Box* %this, double %value) {
entry:
  %_a = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
  store double %value, double* %_a, align 8
}

define void @Square_SetB(%class.Square* %this, double %value) {
entry:
  %_b = getelementptr %class.Square, %class.Square* %this, i32 0, i32 0
  store double %value, double* %_b, align 8
}

define void @Cube_SetC(%class.Cube* %this, double %value) {
entry:
  %box = bitcast %class.Cube* %this to %class.Box*
  call void @Box_SetA(%class.Box* %box, double %value)
  %square = bitcast %class.Cube* %this to %class.Square*
  call void @Square_SetB(%class.Square* %square, double %value)
  %_c = getelementptr %class.Cube, %class.Cube* %this, i32 0, i32 2
  store double %value, double* %_c, align 8
}
```


## 总结
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

Value *getMemberValue(StructType *type, Value *value, int index, std::string name) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    return Builder->CreateLoad(Builder->getDoubleTy(), memberPtr, name + "_value");
}

void setMemberValue(StructType *type, Value *value, int index, std::string name, Value *target) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    Builder->CreateStore(target, memberPtr);
}


StructType *createBoxTy() {
  // create Box class struct
  StructType *Box = StructType::create(*TheContext, "class.Box");
  Box->setBody(Builder->getDoubleTy());

  // create void Box::SetA(double value)
  Function *Box3getEv = createFunc(Builder->getVoidTy(), { PointerType::get(Box, 0), Builder->getDoubleTy()}, "Box_SetA");
  std::vector<std::string> BoxGetFuncArgs;
  BoxGetFuncArgs.push_back("this");
  BoxGetFuncArgs.push_back("value");
  setFuncArgs(Box3getEv, BoxGetFuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Box3getEv);
  Builder->SetInsertPoint(entry);

  Function::arg_iterator getAI = Box3getEv->arg_begin();
  Value *getThis = getAI++;
  Value *value = getAI++;
  setMemberValue(Box, getThis, 0, "_a", value);
  verifyFunction(*Box3getEv);
  return Box;
}

StructType *createSquareTy() {
  // Square class struct
  StructType *Square = StructType::create(*TheContext, "class.Square");
  Square->setBody(Builder->getDoubleTy());

  // create void Square::Square_SetB(double value)
  Function *Square_SetB = createFunc(Builder->getVoidTy(), { PointerType::get(Square, 0), Builder->getDoubleTy()}, "Square_SetB");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  FuncArgs.push_back("value");
  setFuncArgs(Square_SetB, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Square_SetB);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = Square_SetB->arg_begin();
  Value *This = getAI++;
  Value *value = getAI++;
  setMemberValue(Square, This, 0, "_b", value);
  verifyFunction(*Square_SetB);
  return Square;
}

StructType *createCubeTy(StructType *BoxTy, StructType *SquareTy) {
  // Cube class struct
  StructType *Cube = StructType::create(*TheContext, "class.Cube");
  Cube->setBody(BoxTy, SquareTy, Builder->getDoubleTy());

  // create void Cube::Cube_SetC(double value)
  Function *Cube_SetC = createFunc(Builder->getVoidTy(), { PointerType::get(Cube, 0), Builder->getDoubleTy()}, "Cube_SetC");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  FuncArgs.push_back("value");
  setFuncArgs(Cube_SetC, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Cube_SetC);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = Cube_SetC->arg_begin();
  Value *This = getAI++;
  Value *value = getAI++;

  Value *boxObj = Builder->CreateBitCast(This, PointerType::get(BoxTy, 0), "box");
  Function *Box_SetA = TheModule->getFunction("Box_SetA");
  Builder->CreateCall(Box_SetA, {boxObj, value});

  Value *squareObj = Builder->CreateBitCast(This, PointerType::get(SquareTy, 0), "square");
  Function *Square_SetB = TheModule->getFunction("Square_SetB");
  Builder->CreateCall(Square_SetB, {squareObj, value});

  setMemberValue(Cube, This, 2, "_c", value);
  verifyFunction(*Cube_SetC);
  return Cube;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  StructType *BoxTy = createBoxTy();
  StructType *SquareTy = createSquareTy();
  StructType *Cube = createCubeTy(BoxTy, SquareTy);

  TheModule->print(errs(), nullptr);
  return 0;
}
```
### 源码地址
[LLVM IR in Action/Chapter_04/multiple_inheritance](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_04/multiple_inheritance.cpp)