# LLVM in Action:虚函数(virtual method)

标签（空格分隔）： LLVM

---
本文介绍如何生成C++中virtual method对应的LLVM IR.
**先给出demo C++ class**
```
class Square
{
   public:
      double width;
      virtual double getArea() {
          return width * width;
      }

      void set( double dimen) {
         width = dimen;
      }  
};

class Rectangle:public Square {
   public:
      double length;
      double getArea() {
         return width * length;
      }

      void setLength(double len) {
         length = len;
      }
};
```
## 虚函数表vtable
vtable可以看成一个数组，其元素类型为函数指针：
```
int (*p[])(...);
```
对应的LLVM IR:
```
i32 (...)**
```
对应的生成代码：
```
PointerType* createVtable() {
  // function type: int (*f)(...) => i32 (...)
  FunctionType *funcType = FunctionType::get(Builder->getInt32Ty(), {}, true);
  PointerType* vtable = PointerType::get(funcType, 0);
  return PointerType::get(vtable, 0);
}
```
## 创建基类
### 实现基类Square
```
StructType *createSquareTy(PointerType *vtable) {
  StructType *SquareTy = StructType::create(*TheContext, "class.Square");
  SquareTy->setBody({vtable, Builder->getDoubleTy()});
  // getArea
  Function *getArea = createFunc(Builder->getDoubleTy(), {PointerType::get(SquareTy, 0)}, "Square1getArea");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;
  Value *width = getMemberValue(SquareTy, getThis, 1, "width");
  Value *getRet = Builder->CreateMul(width, width);
  Builder->CreateRet(getRet);
  verifyFunction(*getArea);

  // set(double)
  Function *set = createFunc(Builder->getVoidTy(), {PointerType::get(SquareTy, 0), Builder->getDoubleTy()}, "Square1set");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("this");
  SetFuncArgs.push_back("dimen");
  setFuncArgs(set, SetFuncArgs);
  BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", set);
  Builder->SetInsertPoint(setEntry);
  Function::arg_iterator setAI = set->arg_begin();
  Value *setThis = setAI++;
  Value *lenVal = Builder->CreateLoad(Builder->getDoubleTy(), setAI, "dimen_value");
  setMemberValue(SquareTy, setThis, 1, "width", lenVal);
  verifyFunction(*set);
  return SquareTy;
}
```
### 生成的IR
```
// 第一个元素为vptr
%class.Square = type { i32 (...)**, double }

define double @Square1getArea(%class.Square* %this) {
entry:
  %width = getelementptr %class.Square, %class.Square* %this, i32 0, i32 1
  %width_value = load double, double* %width, align 8
  %0 = mul double %width_value, %width_value
  ret double %0
}

define void @Square1set(%class.Square* %this, double %dimen) {
entry:
  %dimen_value = load double, double %dimen, align 8
  %width = getelementptr %class.Square, %class.Square* %this, i32 0, i32 1
  store double %dimen_value, double* %width, align 8
}
```
##创建子类
### 创建子类Rectangle
```
StructType *createRectangleTy(StructType *SquareTy) {
  StructType *Rectangle = StructType::create(*TheContext, "class.Rectangle");
  Rectangle->setBody({SquareTy, Builder->getDoubleTy()});
  
  //getArea
  Function *getArea = createFunc(Builder->getDoubleTy(), {PointerType::get(Rectangle, 0)}, "Rectangle2getArea");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;
  Value *super = Builder->CreateBitCast(getThis, PointerType::get(SquareTy, 0), "square");
  Value *width = getMemberValue(SquareTy, super, 1, "width");
  Value *length = getMemberValue(Rectangle, getThis, 1, "length");
  Value *getRet = Builder->CreateMul(width, length);
  Builder->CreateRet(getRet);
  verifyFunction(*getArea);

  // setLenght(double)
  Function *set = createFunc(Builder->getVoidTy(), {PointerType::get(Rectangle, 0), Builder->getDoubleTy()}, "Rectangle2setLenght");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("this");
  SetFuncArgs.push_back("len");
  setFuncArgs(set, SetFuncArgs);
  BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", set);
  Builder->SetInsertPoint(setEntry);
  Function::arg_iterator setAI = set->arg_begin();
  Value *setThis = setAI++;
  Value *lenVal = Builder->CreateLoad(Builder->getDoubleTy(), setAI, "len_value");
  setMemberValue(Rectangle, setThis, 1, "length", lenVal);
  verifyFunction(*set);
  return Rectangle;
}
```
###生成的IR
```
%class.Square = type { i32 (...)**, double }
%class.Rectangle = type { %class.Square, double }

define double @Rectangle2getArea(%class.Rectangle* %this) {
entry:
  %square = bitcast %class.Rectangle* %this to %class.Square*
  %width = getelementptr %class.Square, %class.Square* %square, i32 0, i32 1
  %width_value = load double, double* %width, align 8
  %length = getelementptr %class.Rectangle, %class.Rectangle* %this, i32 0, i32 1
  %length_value = load double, double* %length, align 8
  %0 = mul double %width_value, %length_value
  ret double %0
}

define void @Rectangle2setLenght(%class.Rectangle* %this, double %len) {
entry:
  %len_value = load double, double %len, align 8
  %length = getelementptr %class.Rectangle, %class.Rectangle* %this, i32 0, i32 1
  store double %len_value, double* %length, align 8
}
```
## 调用虚函数
虚函数调用代码如下所示：
```
double increaseArea(Square *ps) {
   return ps->getArea() + 1;
}
```
### 生成函数
需要先将Square *ps转成vptr_table的地址，然后取出对应的函数指针
```
void increaseAreaP6Square(StructType *SquareTy) {
  Function *increaseArea = createFunc(Builder->getDoubleTy(), {PointerType::get(SquareTy, 0)}, "increaseAreaP6Square");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("ps");
  setFuncArgs(increaseArea, SetFuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", increaseArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator setAI = increaseArea->arg_begin();
  Value *setThis = setAI++;

  PointerType *pSquareTy = PointerType::get(SquareTy, 0);
  Value *thisTmp = Builder->CreateAlloca(pSquareTy);
  Builder->CreateStore(setThis, thisTmp);
  Value *pSquare = Builder->CreateLoad(pSquareTy, thisTmp, "pSquare");

  FunctionType *funcType = FunctionType::get(Builder->getDoubleTy(), {pSquareTy}, false);
  PointerType* pfuncType = PointerType::get(funcType, 0);
  PointerType *SquareVTable = PointerType::get(pfuncType, 0);
  Value *pvtable = Builder->CreateBitCast(pSquare, PointerType::get(SquareVTable, 0), "pSquareVTable");
  Value *vtable = Builder->CreateLoad(SquareVTable, pvtable, "vtable");
  Value *func = Builder->CreateGEP(pfuncType, vtable, Builder->getInt64(0), "increaseArea");
  Value *area = Builder->CreateCall(funcType, func, None, "area");
  Value *result = Builder->CreateFAdd(area, ConstantFP::get(Builder->getDoubleTy(), 1), "result");
  Builder->CreateRet(result);
  verifyFunction(*increaseArea);
}
```
### 生成的IR
```
define double @increaseAreaP6Square(%class.Square* %ps) {
entry:
  %0 = alloca %class.Square*, align 8
  store %class.Square* %ps, %class.Square** %0, align 8
  %pSquare = load %class.Square*, %class.Square** %0, align 8
  %pSquareVTable = bitcast %class.Square* %pSquare to double (%class.Square*)***
  %vtable = load double (%class.Square*)**, double (%class.Square*)*** %pSquareVTable, align 8
  %increaseArea = getelementptr double (%class.Square*)*, double (%class.Square*)** %vtable, i64 0
  %area = call double %increaseArea()
  %result = fadd double %area, 1.000000e+00
  ret double %result
}
```

### 运行程序
执行命令
```
clang++ virtual.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
virtual.cpp
; ModuleID = 'first module'
source_filename = "first module"

%class.Square = type { i32 (...)**, double }
%class.Rectangle = type { %class.Square, double }

define double @Square1getArea(%class.Square* %this) {
entry:
  %width = getelementptr %class.Square, %class.Square* %this, i32 0, i32 1
  %width_value = load double, double* %width, align 8
  %0 = mul double %width_value, %width_value
  ret double %0
}

define void @Square1set(%class.Square* %this, double %dimen) {
entry:
  %dimen_value = load double, double %dimen, align 8
  %width = getelementptr %class.Square, %class.Square* %this, i32 0, i32 1
  store double %dimen_value, double* %width, align 8
}

define double @Rectangle2getArea(%class.Rectangle* %this) {
entry:
  %square = bitcast %class.Rectangle* %this to %class.Square*
  %width = getelementptr %class.Square, %class.Square* %square, i32 0, i32 1
  %width_value = load double, double* %width, align 8
  %length = getelementptr %class.Rectangle, %class.Rectangle* %this, i32 0, i32 1
  %length_value = load double, double* %length, align 8
  %0 = mul double %width_value, %length_value
  ret double %0
}

define void @Rectangle2setLenght(%class.Rectangle* %this, double %len) {
entry:
  %len_value = load double, double %len, align 8
  %length = getelementptr %class.Rectangle, %class.Rectangle* %this, i32 0, i32 1
  store double %len_value, double* %length, align 8
}

define double @increaseAreaP6Square(%class.Square* %ps) {
entry:
  %0 = alloca %class.Square*, align 8
  store %class.Square* %ps, %class.Square** %0, align 8
  %pSquare = load %class.Square*, %class.Square** %0, align 8
  %pSquareVTable = bitcast %class.Square* %pSquare to double (%class.Square*)***
  %vtable = load double (%class.Square*)**, double (%class.Square*)*** %pSquareVTable, align 8
  %increaseArea = getelementptr double (%class.Square*)*, double (%class.Square*)** %vtable, i64 0
  %area = call double %increaseArea()
  %result = fadd double %area, 1.000000e+00
  ret double %result
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

Value *getMemberValue(StructType *type, Value *value, int index, std::string name) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    return Builder->CreateLoad(Builder->getDoubleTy(), memberPtr, name + "_value");
}

void setMemberValue(StructType *type, Value *value, int index, std::string name, Value *target) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    Builder->CreateStore(target, memberPtr);
}


PointerType *createVtable() {
  // function type: int (*f)(...) => i32 (...)
  FunctionType *funcType = FunctionType::get(Builder->getInt32Ty(), {}, true);
  PointerType* vtable = PointerType::get(funcType, 0);
  return PointerType::get(vtable, 0);
}

StructType *createSquareTy(PointerType *vtable) {
  StructType *SquareTy = StructType::create(*TheContext, "class.Square");
  SquareTy->setBody({vtable, Builder->getDoubleTy()});
  // getArea
  Function *getArea = createFunc(Builder->getDoubleTy(), {PointerType::get(SquareTy, 0)}, "Square1getArea");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;
  Value *width = getMemberValue(SquareTy, getThis, 1, "width");
  Value *getRet = Builder->CreateMul(width, width);
  Builder->CreateRet(getRet);
  verifyFunction(*getArea);

  // set(double)
  Function *set = createFunc(Builder->getVoidTy(), {PointerType::get(SquareTy, 0), Builder->getDoubleTy()}, "Square1set");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("this");
  SetFuncArgs.push_back("dimen");
  setFuncArgs(set, SetFuncArgs);
  BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", set);
  Builder->SetInsertPoint(setEntry);
  Function::arg_iterator setAI = set->arg_begin();
  Value *setThis = setAI++;
  Value *lenVal = Builder->CreateLoad(Builder->getDoubleTy(), setAI, "dimen_value");
  setMemberValue(SquareTy, setThis, 1, "width", lenVal);
  verifyFunction(*set);
  return SquareTy;
}

StructType *createRectangleTy(StructType *SquareTy) {
  StructType *Rectangle = StructType::create(*TheContext, "class.Rectangle");
  Rectangle->setBody({SquareTy, Builder->getDoubleTy()});
  
  //getArea
  Function *getArea = createFunc(Builder->getDoubleTy(), {PointerType::get(Rectangle, 0)}, "Rectangle2getArea");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;
  Value *super = Builder->CreateBitCast(getThis, PointerType::get(SquareTy, 0), "square");
  Value *width = getMemberValue(SquareTy, super, 1, "width");
  Value *length = getMemberValue(Rectangle, getThis, 1, "length");
  Value *getRet = Builder->CreateMul(width, length);
  Builder->CreateRet(getRet);
  verifyFunction(*getArea);

  // setLenght(double)
  Function *set = createFunc(Builder->getVoidTy(), {PointerType::get(Rectangle, 0), Builder->getDoubleTy()}, "Rectangle2setLenght");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("this");
  SetFuncArgs.push_back("len");
  setFuncArgs(set, SetFuncArgs);
  BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", set);
  Builder->SetInsertPoint(setEntry);
  Function::arg_iterator setAI = set->arg_begin();
  Value *setThis = setAI++;
  Value *lenVal = Builder->CreateLoad(Builder->getDoubleTy(), setAI, "len_value");
  setMemberValue(Rectangle, setThis, 1, "length", lenVal);
  verifyFunction(*set);
  return Rectangle;
}

void increaseAreaP6Square(StructType *SquareTy) {
  Function *increaseArea = createFunc(Builder->getDoubleTy(), {PointerType::get(SquareTy, 0)}, "increaseAreaP6Square");
  std::vector<std::string> SetFuncArgs;
  SetFuncArgs.push_back("ps");
  setFuncArgs(increaseArea, SetFuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", increaseArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator setAI = increaseArea->arg_begin();
  Value *setThis = setAI++;

  PointerType *pSquareTy = PointerType::get(SquareTy, 0);
  Value *thisTmp = Builder->CreateAlloca(pSquareTy);
  Builder->CreateStore(setThis, thisTmp);
  Value *pSquare = Builder->CreateLoad(pSquareTy, thisTmp, "pSquare");

  FunctionType *funcType = FunctionType::get(Builder->getDoubleTy(), {pSquareTy}, false);
  PointerType* pfuncType = PointerType::get(funcType, 0);
  PointerType *SquareVTable = PointerType::get(pfuncType, 0);
  Value *pvtable = Builder->CreateBitCast(pSquare, PointerType::get(SquareVTable, 0), "pSquareVTable");
  Value *vtable = Builder->CreateLoad(SquareVTable, pvtable, "vtable");
  Value *func = Builder->CreateGEP(pfuncType, vtable, Builder->getInt64(0), "increaseArea");
  Value *area = Builder->CreateCall(funcType, func, None, "area");
  Value *result = Builder->CreateFAdd(area, ConstantFP::get(Builder->getDoubleTy(), 1), "result");
  Builder->CreateRet(result);
  verifyFunction(*increaseArea);
}

int main(int argc, char *argv[]) {
  InitializeModule();

  PointerType* vtable = createVtable();

  StructType *SquareTy = createSquareTy(vtable);
  StructType *RectangleTy = createRectangleTy(SquareTy);
  increaseAreaP6Square(SquareTy);


  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_03/](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_04/virtual.cpp)







