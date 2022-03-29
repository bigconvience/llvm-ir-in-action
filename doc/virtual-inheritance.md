# LLVM in Action:实现虚继承(virtual inheritance)

标签（空格分隔）： LLVM

---
本文介绍如何生成C++中现虚继承(virtual inheritance)对应的LLVM IR.
**先给出demo C++ class**
```
class BaseA
{
public:
    int a;

};

class BaseB: virtual BaseA
{
public:
    int b;
    int sumB() {
      return a + b;
    }
};

class BaseC: virtual BaseA
{
public:
    int c;
    int sumC() {
      return a + c;
    }
};

class Derived:
    public BaseB,
    public BaseC
{
public:
   int d;
   int sumDer() {
    return sumB() + sumC() + d;
   } 
};
```
## 创建基类
### 实现基类BaseA
```
StructType *CreateBaseA() {
  StructType *BaseA = StructType::create(*TheContext, "class.BaseA");
  BaseA->setBody(Builder->getInt32Ty());

  return BaseA;
}
```
### 生成的IR
```
%class.BaseA = type { i32 }
```

### 实现基类BaseB
```
StructType *CreateBaseB(PointerType *vtable, StructType *BaseA) {
  StructType *BaseB = StructType::create(*TheContext, "class.BaseB");
  BaseB->setBody({vtable, Builder->getInt32Ty(), BaseA}, true);

  // BaseB4sumBEv 
  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(BaseB, 0)}, "BaseB4sumBEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pBaseB = PointerType::get(BaseB, 0);
  Value *thisTmp = Builder->CreateAlloca(pBaseB);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrBaseB = Builder->CreateLoad(pBaseB, thisTmp, "ptrBaseB");

  Type *i8Ty = Builder->getInt8Ty();
  Type *i64Ty = Builder->getInt64Ty();
  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  PointerType *pi64Ty = PointerType::get(i64Ty, 0);
  PointerType *ppi8Ty = PointerType::get(pi8Ty, 0);
  Value *ppi8 = Builder->CreateBitCast(ptrBaseB, ppi8Ty);
  Value *pi8Value = Builder->CreateLoad(pi8Ty, ppi8);
  Value *tmp6 = Builder->CreateGEP(i8Ty, pi8Value, Builder->getInt64(-24), "tmp6");
  Value *temp7 = Builder->CreateBitCast(tmp6, pi64Ty, "temp7");
  Value *temp8 = Builder->CreateLoad(i64Ty, temp7, "temp8");
  Value *temp9 = Builder->CreateBitCast(ptrBaseB, pi8Ty, "temp9");
  Value *temp10 = Builder->CreateGEP(i8Ty, temp9, temp8, "temp10");
  Value *ptrBaseA = Builder->CreateBitCast(temp10, PointerType::get(BaseA, 0), "baseA");
  Value *a = getMemberValue(BaseA, ptrBaseA, 0, "a");
  Value *b = getMemberValue(BaseB, ptrBaseB, 1, "b");
  Value *ret = Builder->CreateAdd(a, b, "ret");
  Builder->CreateRet(ret);
  return BaseB;
}
```
### 生成的IR
```
%class.BaseB = type <{ i32 (...)**, i32, %class.BaseA }>

define i32 @BaseB4sumBEv(%class.BaseB* %this) {
entry:
  %0 = alloca %class.BaseB*, align 8
  store %class.BaseB* %this, %class.BaseB** %0, align 8
  %ptrBaseB = load %class.BaseB*, %class.BaseB** %0, align 8
  %1 = bitcast %class.BaseB* %ptrBaseB to i8**
  %2 = load i8*, i8** %1, align 8
  %tmp6 = getelementptr i8, i8* %2, i64 -24
  %temp7 = bitcast i8* %tmp6 to i64*
  %temp8 = load i64, i64* %temp7, align 4
  %temp9 = bitcast %class.BaseB* %ptrBaseB to i8*
  %temp10 = getelementptr i8, i8* %temp9, i64 %temp8
  %baseA = bitcast i8* %temp10 to %class.BaseA*
  %a = getelementptr %class.BaseA, %class.BaseA* %baseA, i32 0, i32 0
  %a_value = load double, i32* %a, align 8
  %b = getelementptr %class.BaseB, %class.BaseB* %ptrBaseB, i32 0, i32 1
  %b_value = load double, i32* %b, align 8
  %ret = add double %a_value, %b_value
  ret double %ret
}
```
### 实现基类BaseC
```
StructType *CreateBaseC(PointerType *vtable, StructType *BaseA) {
  StructType *BaseC = StructType::create(*TheContext, "class.BaseC");
  BaseC->setBody({vtable, Builder->getInt32Ty(), BaseA}, true);

  // BaseC4sumCEv 
  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(BaseC, 0)}, "BaseC4sumCEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pBaseC = PointerType::get(BaseC, 0);
  Value *thisTmp = Builder->CreateAlloca(pBaseC);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrBaseC = Builder->CreateLoad(pBaseC, thisTmp, "ptrBaseC");

  Type *i8Ty = Builder->getInt8Ty();
  Type *i64Ty = Builder->getInt64Ty();
  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  PointerType *pi64Ty = PointerType::get(i64Ty, 0);
  PointerType *ppi8Ty = PointerType::get(pi8Ty, 0);
  Value *ppi8 = Builder->CreateBitCast(ptrBaseC, ppi8Ty);
  Value *pi8Value = Builder->CreateLoad(pi8Ty, ppi8);
  Value *tmp6 = Builder->CreateGEP(i8Ty, pi8Value, Builder->getInt64(-24), "tmp6");
  Value *temp7 = Builder->CreateBitCast(tmp6, pi64Ty, "temp7");
  Value *temp8 = Builder->CreateLoad(i64Ty, temp7, "temp8");
  Value *temp9 = Builder->CreateBitCast(ptrBaseC, pi8Ty, "temp9");
  Value *temp10 = Builder->CreateGEP(i8Ty, temp9, temp8, "temp10");
  Value *ptrBaseA = Builder->CreateBitCast(temp10, PointerType::get(BaseA, 0), "baseA");
  Value *a = getMemberValue(BaseA, ptrBaseA, 0, "a");
  Value *c = getMemberValue(BaseC, ptrBaseC, 1, "c");
  Value *ret = Builder->CreateAdd(a, c, "ret");
  Builder->CreateRet(ret);
  return BaseC;
}
```
### 生成的IR
```
%class.BaseC = type <{ i32 (...)**, i32, %class.BaseA }>

define i32 @BaseC4sumCEv(%class.BaseC* %this) {
entry:
  %0 = alloca %class.BaseC*, align 8
  store %class.BaseC* %this, %class.BaseC** %0, align 8
  %ptrBaseC = load %class.BaseC*, %class.BaseC** %0, align 8
  %1 = bitcast %class.BaseC* %ptrBaseC to i8**
  %2 = load i8*, i8** %1, align 8
  %tmp6 = getelementptr i8, i8* %2, i64 -24
  %temp7 = bitcast i8* %tmp6 to i64*
  %temp8 = load i64, i64* %temp7, align 4
  %temp9 = bitcast %class.BaseC* %ptrBaseC to i8*
  %temp10 = getelementptr i8, i8* %temp9, i64 %temp8
  %baseA = bitcast i8* %temp10 to %class.BaseA*
  %a = getelementptr %class.BaseA, %class.BaseA* %baseA, i32 0, i32 0
  %a_value = load double, i32* %a, align 8
  %c = getelementptr %class.BaseC, %class.BaseC* %ptrBaseC, i32 0, i32 1
  %c_value = load double, i32* %c, align 8
  %ret = add double %a_value, %c_value
  ret double %ret
}

```
##创建子类
### 创建子类Derived
```
StructType *CreateBaseDerived(PointerType *vtable, StructType *BaseA, StructType *BaseB, StructType *BaseC) {
  StructType *BaseBBase = StructType::create(*TheContext, "class.BaseB.Base");
  BaseBBase->setBody({vtable, Builder->getInt32Ty()}, true);
  StructType *BaseCBase = StructType::create(*TheContext, "class.BaseC.Base");
  BaseCBase->setBody({vtable, Builder->getInt32Ty()}, true);
  StructType *Derived = StructType::create(*TheContext, "class.Derived");
  
  Type *i8Ty = Builder->getInt8Ty();
  ArrayType *array = ArrayType::get(i8Ty, 4);
  Derived->setBody({BaseBBase, array, BaseCBase, BaseA, array});

  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(Derived, 0)}, "Derived6sumDerEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pDerived = PointerType::get(Derived, 0);
  Value *thisTmp = Builder->CreateAlloca(pDerived);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrDerived = Builder->CreateLoad(pDerived, thisTmp, "ptrDerived");
  Value *baseB = Builder->CreateBitCast(ptrDerived, PointerType::get(BaseB, 0), "baseB");
  Function *BaseB4sumBEv = TheModule->getFunction("BaseB4sumBEv");
  Value *sumB = Builder->CreateCall(BaseB4sumBEv, {baseB}, "sumB");


  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  Value *ptrI8 = Builder->CreateBitCast(ptrDerived, pi8Ty, "ptrI8");
  Value *tmp7 = Builder->CreateGEP(i8Ty, ptrI8, Builder->getInt64(16), "temp7");
  Value *baseC = Builder->CreateBitCast(tmp7, PointerType::get(BaseC, 0), "baseC");
  Function *BaseC4sumCEv = TheModule->getFunction("BaseC4sumCEv");
  Value *sumC = Builder->CreateCall(BaseC4sumCEv, {baseC}, "sumC");
  Value *sum1 = Builder->CreateAdd(sumB, sumC, "sum1");
  Value *ptrd = Builder->CreateGEP(Derived, ptrDerived, {Builder->getInt32(0), Builder->getInt32(3)}, "ptrd");
  Value *d = Builder->CreateLoad(Builder->getInt32Ty(), ptrd, "d");
  Value *ret = Builder->CreateAdd(sum1, d);
  Builder->CreateRet(ret);

  return Derived;
}
```
###生成的IR
```
%class.Derived = type { %class.BaseB.Base, [4 x i8], %class.BaseC.Base, %class.BaseA, [4 x i8] }
%class.BaseB.Base = type <{ i32 (...)**, i32 }>
%class.BaseC.Base = type <{ i32 (...)**, i32 }>

define i32 @Derived6sumDerEv(%class.Derived* %this) {
entry:
  %0 = alloca %class.Derived*, align 8
  store %class.Derived* %this, %class.Derived** %0, align 8
  %ptrDerived = load %class.Derived*, %class.Derived** %0, align 8
  %baseB = bitcast %class.Derived* %ptrDerived to %class.BaseB*
  %sumB = call i32 @BaseB4sumBEv(%class.BaseB* %baseB)
  %ptrI8 = bitcast %class.Derived* %ptrDerived to i8*
  %temp7 = getelementptr i8, i8* %ptrI8, i64 16
  %baseC = bitcast i8* %temp7 to %class.BaseC*
  %sumC = call i32 @BaseC4sumCEv(%class.BaseC* %baseC)
  %sum1 = add i32 %sumB, %sumC
  %ptrd = getelementptr %class.Derived, %class.Derived* %ptrDerived, i32 0, i32 3
  %d = load i32, %class.BaseA* %ptrd, align 4
  %1 = add i32 %sum1, %d
  ret i32 %1
}
```

### 运行程序
执行命令
```
clang++ virtual_inheritance.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first module'
source_filename = "first module"

%class.BaseB = type <{ i32 (...)**, i32, %class.BaseA }>
%class.BaseA = type { i32 }
%class.BaseC = type <{ i32 (...)**, i32, %class.BaseA }>
%class.Derived = type { %class.BaseB.Base, [4 x i8], %class.BaseC.Base, %class.BaseA, [4 x i8] }
%class.BaseB.Base = type <{ i32 (...)**, i32 }>
%class.BaseC.Base = type <{ i32 (...)**, i32 }>

define i32 @BaseB4sumBEv(%class.BaseB* %this) {
entry:
  %0 = alloca %class.BaseB*, align 8
  store %class.BaseB* %this, %class.BaseB** %0, align 8
  %ptrBaseB = load %class.BaseB*, %class.BaseB** %0, align 8
  %1 = bitcast %class.BaseB* %ptrBaseB to i8**
  %2 = load i8*, i8** %1, align 8
  %tmp6 = getelementptr i8, i8* %2, i64 -24
  %temp7 = bitcast i8* %tmp6 to i64*
  %temp8 = load i64, i64* %temp7, align 4
  %temp9 = bitcast %class.BaseB* %ptrBaseB to i8*
  %temp10 = getelementptr i8, i8* %temp9, i64 %temp8
  %baseA = bitcast i8* %temp10 to %class.BaseA*
  %a = getelementptr %class.BaseA, %class.BaseA* %baseA, i32 0, i32 0
  %a_value = load double, i32* %a, align 8
  %b = getelementptr %class.BaseB, %class.BaseB* %ptrBaseB, i32 0, i32 1
  %b_value = load double, i32* %b, align 8
  %ret = add double %a_value, %b_value
  ret double %ret
}

define i32 @BaseC4sumCEv(%class.BaseC* %this) {
entry:
  %0 = alloca %class.BaseC*, align 8
  store %class.BaseC* %this, %class.BaseC** %0, align 8
  %ptrBaseC = load %class.BaseC*, %class.BaseC** %0, align 8
  %1 = bitcast %class.BaseC* %ptrBaseC to i8**
  %2 = load i8*, i8** %1, align 8
  %tmp6 = getelementptr i8, i8* %2, i64 -24
  %temp7 = bitcast i8* %tmp6 to i64*
  %temp8 = load i64, i64* %temp7, align 4
  %temp9 = bitcast %class.BaseC* %ptrBaseC to i8*
  %temp10 = getelementptr i8, i8* %temp9, i64 %temp8
  %baseA = bitcast i8* %temp10 to %class.BaseA*
  %a = getelementptr %class.BaseA, %class.BaseA* %baseA, i32 0, i32 0
  %a_value = load double, i32* %a, align 8
  %c = getelementptr %class.BaseC, %class.BaseC* %ptrBaseC, i32 0, i32 1
  %c_value = load double, i32* %c, align 8
  %ret = add double %a_value, %c_value
  ret double %ret
}

define i32 @Derived6sumDerEv(%class.Derived* %this) {
entry:
  %0 = alloca %class.Derived*, align 8
  store %class.Derived* %this, %class.Derived** %0, align 8
  %ptrDerived = load %class.Derived*, %class.Derived** %0, align 8
  %baseB = bitcast %class.Derived* %ptrDerived to %class.BaseB*
  %sumB = call i32 @BaseB4sumBEv(%class.BaseB* %baseB)
  %ptrI8 = bitcast %class.Derived* %ptrDerived to i8*
  %temp7 = getelementptr i8, i8* %ptrI8, i64 16
  %baseC = bitcast i8* %temp7 to %class.BaseC*
  %sumC = call i32 @BaseC4sumCEv(%class.BaseC* %baseC)
  %sum1 = add i32 %sumB, %sumC
  %ptrd = getelementptr %class.Derived, %class.Derived* %ptrDerived, i32 0, i32 3
  %d = load i32, %class.BaseA* %ptrd, align 4
  %1 = add i32 %sum1, %d
  ret i32 %1
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

StructType *CreateBaseA() {
  StructType *BaseA = StructType::create(*TheContext, "class.BaseA");
  BaseA->setBody(Builder->getInt32Ty());

  return BaseA;
}

StructType *CreateBaseB(PointerType *vtable, StructType *BaseA) {
  StructType *BaseB = StructType::create(*TheContext, "class.BaseB");
  BaseB->setBody({vtable, Builder->getInt32Ty(), BaseA}, true);

  // BaseB4sumBEv 
  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(BaseB, 0)}, "BaseB4sumBEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pBaseB = PointerType::get(BaseB, 0);
  Value *thisTmp = Builder->CreateAlloca(pBaseB);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrBaseB = Builder->CreateLoad(pBaseB, thisTmp, "ptrBaseB");

  Type *i8Ty = Builder->getInt8Ty();
  Type *i64Ty = Builder->getInt64Ty();
  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  PointerType *pi64Ty = PointerType::get(i64Ty, 0);
  PointerType *ppi8Ty = PointerType::get(pi8Ty, 0);
  Value *ppi8 = Builder->CreateBitCast(ptrBaseB, ppi8Ty);
  Value *pi8Value = Builder->CreateLoad(pi8Ty, ppi8);
  Value *tmp6 = Builder->CreateGEP(i8Ty, pi8Value, Builder->getInt64(-24), "tmp6");
  Value *temp7 = Builder->CreateBitCast(tmp6, pi64Ty, "temp7");
  Value *temp8 = Builder->CreateLoad(i64Ty, temp7, "temp8");
  Value *temp9 = Builder->CreateBitCast(ptrBaseB, pi8Ty, "temp9");
  Value *temp10 = Builder->CreateGEP(i8Ty, temp9, temp8, "temp10");
  Value *ptrBaseA = Builder->CreateBitCast(temp10, PointerType::get(BaseA, 0), "baseA");
  Value *a = getMemberValue(BaseA, ptrBaseA, 0, "a");
  Value *b = getMemberValue(BaseB, ptrBaseB, 1, "b");
  Value *ret = Builder->CreateAdd(a, b, "ret");
  Builder->CreateRet(ret);
  return BaseB;
}

StructType *CreateBaseC(PointerType *vtable, StructType *BaseA) {
  StructType *BaseC = StructType::create(*TheContext, "class.BaseC");
  BaseC->setBody({vtable, Builder->getInt32Ty(), BaseA}, true);

  // BaseC4sumCEv 
  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(BaseC, 0)}, "BaseC4sumCEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pBaseC = PointerType::get(BaseC, 0);
  Value *thisTmp = Builder->CreateAlloca(pBaseC);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrBaseC = Builder->CreateLoad(pBaseC, thisTmp, "ptrBaseC");

  Type *i8Ty = Builder->getInt8Ty();
  Type *i64Ty = Builder->getInt64Ty();
  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  PointerType *pi64Ty = PointerType::get(i64Ty, 0);
  PointerType *ppi8Ty = PointerType::get(pi8Ty, 0);
  Value *ppi8 = Builder->CreateBitCast(ptrBaseC, ppi8Ty);
  Value *pi8Value = Builder->CreateLoad(pi8Ty, ppi8);
  Value *tmp6 = Builder->CreateGEP(i8Ty, pi8Value, Builder->getInt64(-24), "tmp6");
  Value *temp7 = Builder->CreateBitCast(tmp6, pi64Ty, "temp7");
  Value *temp8 = Builder->CreateLoad(i64Ty, temp7, "temp8");
  Value *temp9 = Builder->CreateBitCast(ptrBaseC, pi8Ty, "temp9");
  Value *temp10 = Builder->CreateGEP(i8Ty, temp9, temp8, "temp10");
  Value *ptrBaseA = Builder->CreateBitCast(temp10, PointerType::get(BaseA, 0), "baseA");
  Value *a = getMemberValue(BaseA, ptrBaseA, 0, "a");
  Value *c = getMemberValue(BaseC, ptrBaseC, 1, "c");
  Value *ret = Builder->CreateAdd(a, c, "ret");
  Builder->CreateRet(ret);
  return BaseC;
}

StructType *CreateBaseDerived(PointerType *vtable, StructType *BaseA, StructType *BaseB, StructType *BaseC) {
  StructType *BaseBBase = StructType::create(*TheContext, "class.BaseB.Base");
  BaseBBase->setBody({vtable, Builder->getInt32Ty()}, true);
  StructType *BaseCBase = StructType::create(*TheContext, "class.BaseC.Base");
  BaseCBase->setBody({vtable, Builder->getInt32Ty()}, true);
  StructType *Derived = StructType::create(*TheContext, "class.Derived");
  
  Type *i8Ty = Builder->getInt8Ty();
  ArrayType *array = ArrayType::get(i8Ty, 4);
  Derived->setBody({BaseBBase, array, BaseCBase, BaseA, array});

  Function *getArea = createFunc(Builder->getInt32Ty(), {PointerType::get(Derived, 0)}, "Derived6sumDerEv");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("this");
  setFuncArgs(getArea, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", getArea);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator getAI = getArea->arg_begin();
  Value *getThis = getAI;

  PointerType *pDerived = PointerType::get(Derived, 0);
  Value *thisTmp = Builder->CreateAlloca(pDerived);
  Builder->CreateStore(getThis, thisTmp);
  Value *ptrDerived = Builder->CreateLoad(pDerived, thisTmp, "ptrDerived");
  Value *baseB = Builder->CreateBitCast(ptrDerived, PointerType::get(BaseB, 0), "baseB");
  Function *BaseB4sumBEv = TheModule->getFunction("BaseB4sumBEv");
  Value *sumB = Builder->CreateCall(BaseB4sumBEv, {baseB}, "sumB");


  PointerType *pi8Ty = PointerType::get(i8Ty, 0);
  Value *ptrI8 = Builder->CreateBitCast(ptrDerived, pi8Ty, "ptrI8");
  Value *tmp7 = Builder->CreateGEP(i8Ty, ptrI8, Builder->getInt64(16), "temp7");
  Value *baseC = Builder->CreateBitCast(tmp7, PointerType::get(BaseC, 0), "baseC");
  Function *BaseC4sumCEv = TheModule->getFunction("BaseC4sumCEv");
  Value *sumC = Builder->CreateCall(BaseC4sumCEv, {baseC}, "sumC");
  Value *sum1 = Builder->CreateAdd(sumB, sumC, "sum1");
  Value *ptrd = Builder->CreateGEP(Derived, ptrDerived, {Builder->getInt32(0), Builder->getInt32(3)}, "ptrd");
  Value *d = Builder->CreateLoad(Builder->getInt32Ty(), ptrd, "d");
  Value *ret = Builder->CreateAdd(sum1, d);
  Builder->CreateRet(ret);

  return Derived;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  PointerType* vtable = createVtable();
  StructType *BaseA = CreateBaseA();
  StructType *BaseB = CreateBaseB(vtable, BaseA);
  StructType *BaseC = CreateBaseC(vtable, BaseA);
  CreateBaseDerived(vtable, BaseA, BaseB, BaseC);

  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 源码地址

[LLVM IR in Action/Chapter_04/virtual_inheritance](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_04/virtual_inheritance.cpp)












