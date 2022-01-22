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