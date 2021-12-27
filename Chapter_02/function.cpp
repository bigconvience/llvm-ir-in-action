#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <vector>
using namespace llvm;

static LLVMContext TheContext;
static LLVMContext& getGlobalContext() {
    return TheContext;
}
static LLVMContext &Context = getGlobalContext();
static Module *ModuleOb = new Module("my compiler", Context);

Function *createFunc(IRBuilder<> &Builder, Type *RetTy, ArrayRef<Type *> Params, std::string Name, bool isVarArg = false) {
  FunctionType *funcType = llvm::FunctionType::get(RetTy, Params, isVarArg);
  Function *fooFunc = llvm::Function::Create(
      funcType, llvm::Function::PrivateLinkage, Name, ModuleOb);
  return fooFunc;
}

void setFuncArgs(Function *Func, std::vector<std::string> FuncArgs) {
  unsigned Idx = 0;
  Function::arg_iterator AI, AE;
  for(AI = Func->arg_begin(), AE = Func->arg_end(); AI != AE; ++AI, ++Idx) {
    AI->setName(FuncArgs[Idx]);
  }
}

GlobalVariable *createGlob(IRBuilder<> &Builder, Type *Type, std::string Name) {
  ModuleOb->getOrInsertGlobal(Name, Type);
  GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
  return gVar;
}

int main(int argc, char *argv[]) {
  static IRBuilder<> Builder(Context);
  FunctionType *funcType = FunctionType::get(Builder.getInt32Ty(), {Builder.getInt8PtrTy()}, false);
  GlobalVariable *gVar = createGlob(Builder, funcType, "variable");
  
  Function *barFunc = createFunc(Builder, Builder.getInt32Ty(), {Builder.getInt32Ty()}, "Bar");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("value");
  setFuncArgs(barFunc, FuncArgs);
  BasicBlock *entry = BasicBlock::Create(getGlobalContext(), "entry", barFunc);
  Builder.SetInsertPoint(entry);

 
  Builder.CreateRet(Builder.getInt32(17));
  
  Function *fooFunc = createFunc(Builder, Builder.getInt32Ty(), {Builder.getInt32Ty()}, "Foo");
  setFuncArgs(fooFunc, FuncArgs);

  Type *a = Builder.getDoubleTy();
  StructType *point = StructType::create(getGlobalContext(), "Point");
  point->setBody({a, a, a});
  
  Function *foo1Func = createFunc(Builder, point, {point, point}, "add_points");
  std::vector<std::string> PointArgs;
  PointArgs.push_back("a");
  PointArgs.push_back("b");
  setFuncArgs(foo1Func, PointArgs);
  
  verifyFunction(*barFunc);
  ModuleOb->print(errs(), nullptr);
  return 0;
}
