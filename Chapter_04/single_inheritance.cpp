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

StructType *createSquareTy(StructType *BoxTy) {
  // Square class struct
  StructType *Square = StructType::create(*TheContext, "class.Square");
  Square->setBody(BoxTy, Builder->getDoubleTy());

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
  Value *baseObj = Builder->CreateBitCast(This, PointerType::get(BoxTy, 0), "box");
  Function *Box_SetA = TheModule->getFunction("Box_SetA");
  Builder->CreateCall(Box_SetA, {baseObj, value});
  setMemberValue(Square, This, 1, "_b", value);
  verifyFunction(*Square_SetB);
  return Square;
}


int main(int argc, char *argv[]) {
  InitializeModule();
  StructType *BoxTy = createBoxTy();
  StructType *SquareTy = createSquareTy(BoxTy);

  TheModule->print(errs(), nullptr);
  return 0;
}