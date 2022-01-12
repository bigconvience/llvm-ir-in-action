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


PointerType* createVtable() {
  // function type: int (*f)(...) => i32 (...)
  FunctionType *funcType = FunctionType::get(Builder->getInt32Ty(), {}, true);
  PointerType* vtable = PointerType::get(funcType, 0);
  return PointerType::get(vtable, 0);
}

int main(int argc, char *argv[]) {
  InitializeModule();

  PointerType* vtable = createVtable();


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

  Value *fooBar = Builder->CreateAlloca(vtable, nullptr, "vtable");

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
  verifyFunction(*Box3setEddd);

  TheModule->print(errs(), nullptr);
  return 0;
}