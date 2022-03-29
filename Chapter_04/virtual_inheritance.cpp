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