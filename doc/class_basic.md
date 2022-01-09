# LLVM in Action:create class

标签（空格分隔）： 未分类

---

本文介绍如何生成C++中class对应的LLVM IR

## class的LLVM IR表示
class可以用一个含有相同成员变量的struct来表示, class中的方法可以看成是一个函数，其第一个参数是一个指向该struct类型的指针，下面定义一个class:
```
class Box
{
   public:
      double length;  
      double breadth;  
      double height; 
      double get(void);
      void set( double len, double bre, double hei );
};

double Box::get(void)
{
    return length * breadth * height;
}
 
void Box::set( double len, double bre, double hei)
{
    length = len;
    breadth = bre;
    height = hei;
}
```
### 定义Box struct
先创建一个包含三个double类型的StructType:
```
StructType *Box = StructType::create(*TheContext, "class.Box");
Box->setBody({Builder->getDoubleTy(), Builder->getDoubleTy(), Builder->getDoubleTy()});
```
对应的IR:
```
%class.Box = type { double, double, double }
```
### 创建Box的方法
**double Box::get(void)**
```
double Box::get(void)
{
    return length * breadth * height;
}
```
该方法对应的C语言函数可为：
```
double Box3getEv(Box *this) {
    double temp = this->length * this->breadth;
    double ret = temp * this->height;
    return ret;
}
```
对应的LLVM代码：
```
// 访问Box中的成员变量
Value *getMemberValue(StructType *type, Value *value, int index, std::string name) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    return Builder->CreateLoad(Builder->getDoubleTy(), memberPtr, name + "_value");
}

// 创建函数
Function *Box3getEv = createFunc(Builder->getDoubleTy(), { PointerType::get(Box, 0) }, "Box3getEv");
std::vector<std::string> BoxGetFuncArgs;
BoxGetFuncArgs.push_back("this");
setFuncArgs(Box3getEv, BoxGetFuncArgs);

BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Box3getEv);
Builder->SetInsertPoint(entry);

Function::arg_iterator getAI = Box3getEv->arg_begin();
Value *getThis = getAI;

Value *member_length = getMemberValue(Box, getThis, 0, "length");
Value *member_breadth = getMemberValue(Box, getThis, 1, "breadth");
Value *member_height = getMemberValue(Box, getThis, 2, "height");
Value *temp = Builder->CreateMul(member_length, member_breadth);
Value *getRet = Builder->CreateMul(temp, member_height);
Builder->CreateRet(getRet);
```
生成的LLRM IR:
```
%class.Box = type { double, double, double }

define double @Box3getEv(%class.Box* %this) {
entry:
  %length = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
  %length_value = load double, double* %length, align 8
  %breadth = getelementptr %class.Box, %class.Box* %this, i32 0, i32 1
  %breadth_value = load double, double* %breadth, align 8
  %height = getelementptr %class.Box, %class.Box* %this, i32 0, i32 2
  %height_value = load double, double* %height, align 8
  %0 = mul double %length_value, %breadth_value
  %1 = mul double %0, %height_value
  ret double %1
}
```

**void Box::set(double len, double bre, double hei)** 
```
void Box::set(double len, double bre, double hei)
{
    length = len;
    breadth = bre;
    height = hei;
}
```
该方法对应的C函数为：
```
void Box3setEddd(Box *this, double len, double bre, double hei)
{
    length = len;
    breadth = bre;
    height = hei;
}
```
对应的LLVM代码：
```
void setMemberValue(StructType *type, Value *value, int index, std::string name, Value *target) {
    Value *memberPtr = Builder->CreateGEP(type, value, {Builder->getInt32(0), Builder->getInt32(index)}, name);
    Builder->CreateStore(target, memberPtr);
}

// void Box::set(double len, double bre, double hei)
Type *memberType = Builder->getDoubleTy();
Type *retType = Builder->getVoidTy();
Function *Box3setEddd = createFunc(retType, { PointerType::get(Box, 0), memberType, memberType, memberType }, "Box3setEddd");
std::vector<std::string> Box3setEdddArgs;
Box3setEdddArgs.push_back("this");
Box3setEdddArgs.push_back("len");
Box3setEdddArgs.push_back("bre");
Box3setEdddArgs.push_back("hei");
setFuncArgs(Box3setEddd, Box3setEdddArgs);

BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", Box3setEddd);
Builder->SetInsertPoint(setEntry);

Function::arg_iterator setAI = Box3setEddd->arg_begin();
Value *setThis = setAI++;
Value *lenVal = Builder->CreateLoad(memberType, setAI++, "lenVal");
Value *breVal = Builder->CreateLoad(memberType, setAI++, "breVal");
Value *heiVal = Builder->CreateLoad(memberType, setAI++, "heiVal");
setMemberValue(Box, setThis, 0, "length", lenVal);
setMemberValue(Box, setThis, 1, "breadth", breVal);
setMemberValue(Box, setThis, 2, "height", heiVal);
verifyFunction(*Box3setEddd);
```
生成的LLRM IR:
```
define void @Box3setEddd(%class.Box* %this, double %len, double %bre, double %hei) {
entry:
%lenVal = load double, double %len, align 8
%breVal = load double, double %bre, align 8
%heiVal = load double, double %hei, align 8
%length = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
store double %lenVal, double* %length, align 8
%breadth = getelementptr %class.Box, %class.Box* %this, i32 0, i32 1
store double %breVal, double* %breadth, align 8
%height = getelementptr %class.Box, %class.Box* %this, i32 0, i32 2
store double %heiVal, double* %height, align 8
}
```
### 运行程序
执行命令
```
clang++ class.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
source_filename = "first modlue"

%class.Box = type { double, double, double }

define double @Box3getEv(%class.Box* %this) {
entry:
  %length = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
  %length_value = load double, double* %length, align 8
  %breadth = getelementptr %class.Box, %class.Box* %this, i32 0, i32 1
  %breadth_value = load double, double* %breadth, align 8
  %height = getelementptr %class.Box, %class.Box* %this, i32 0, i32 2
  %height_value = load double, double* %height, align 8
  %0 = mul double %length_value, %breadth_value
  %1 = mul double %0, %height_value
  ret double %1
}

define void @Box3setEddd(%class.Box* %this, double %len, double %bre, double %hei) {
entry:
  %lenVal = load double, double %len, align 8
  %breVal = load double, double %bre, align 8
  %heiVal = load double, double %hei, align 8
  %length = getelementptr %class.Box, %class.Box* %this, i32 0, i32 0
  store double %lenVal, double* %length, align 8
  %breadth = getelementptr %class.Box, %class.Box* %this, i32 0, i32 1
  store double %breVal, double* %breadth, align 8
  %height = getelementptr %class.Box, %class.Box* %this, i32 0, i32 2
  store double %heiVal, double* %height, align 8
}
```
## 总结
### Name Mangling
[https://www.avabodh.com/cxxin/namemangling.html](https://www.avabodh.com/cxxin/namemangling.html)和[The Secret Life of C++: Symbol Mangling](http://web.mit.edu/tibbetts/Public/inside-c/www/mangling.html#mangling-clang-s)介绍了CPP的函数编译后的符号名称

### 源码
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


int main(int argc, char *argv[]) {
  InitializeModule();

  // create Box class struct
  StructType *Box = StructType::create(*TheContext, "class.Box");
  Box->setBody({Builder->getDoubleTy(), Builder->getDoubleTy(), Builder->getDoubleTy()});

  // create double Box::get(void)
  Function *Box3getEv = createFunc(Builder->getDoubleTy(), { PointerType::get(Box, 0) }, "Box3getEv");
  std::vector<std::string> BoxGetFuncArgs;
  BoxGetFuncArgs.push_back("this");
  setFuncArgs(Box3getEv, BoxGetFuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", Box3getEv);
  Builder->SetInsertPoint(entry);

  Function::arg_iterator getAI = Box3getEv->arg_begin();
  Value *getThis = getAI;

  Value *member_length = getMemberValue(Box, getThis, 0, "length");
  Value *member_breadth = getMemberValue(Box, getThis, 1, "breadth");
  Value *member_height = getMemberValue(Box, getThis, 2, "height");
  Value *temp = Builder->CreateMul(member_length, member_breadth);
  Value *getRet = Builder->CreateMul(temp, member_height);
  Builder->CreateRet(getRet);
  verifyFunction(*Box3getEv);

  // void Box::set(double len, double bre, double hei)
  Type *memberType = Builder->getDoubleTy();
  Type *retType = Builder->getVoidTy();
  Function *Box3setEddd = createFunc(retType, { PointerType::get(Box, 0), memberType, memberType, memberType }, "Box3setEddd");
  std::vector<std::string> Box3setEdddArgs;
  Box3setEdddArgs.push_back("this");
  Box3setEdddArgs.push_back("len");
  Box3setEdddArgs.push_back("bre");
  Box3setEdddArgs.push_back("hei");
  setFuncArgs(Box3setEddd, Box3setEdddArgs);

  BasicBlock *setEntry = BasicBlock::Create(*TheContext, "entry", Box3setEddd);
  Builder->SetInsertPoint(setEntry);

  Function::arg_iterator setAI = Box3setEddd->arg_begin();
  Value *setThis = setAI++;
  Value *lenVal = Builder->CreateLoad(memberType, setAI++, "lenVal");
  Value *breVal = Builder->CreateLoad(memberType, setAI++, "breVal");
  Value *heiVal = Builder->CreateLoad(memberType, setAI++, "heiVal");
  setMemberValue(Box, setThis, 0, "length", lenVal);
  setMemberValue(Box, setThis, 1, "breadth", breVal);
  setMemberValue(Box, setThis, 2, "height", heiVal);
  verifyFunction(*Box3setEddd);

  TheModule->print(errs(), nullptr);
  return 0;
}
```
### 源码地址

[LLVM IR in Action/Chapter_04/class.cpp](https://github.com/bigconvience/llvm-ir-in-action/blob/main/Chapter_04/class.cpp)