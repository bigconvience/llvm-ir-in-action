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