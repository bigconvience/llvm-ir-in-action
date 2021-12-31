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

int main(int argc, char *argv[]) {
  InitializeModule();
  StructType *Foo = StructType::create(*TheContext, "Foo");
  Foo->setBody({Builder->getInt32Ty(), Builder->getInt8PtrTy(), Builder->getDoubleTy()});

  GlobalVariable *gVar = createGlob(Foo, "foo_struct");

  Function *fooFunc = createFunc(PointerType::get(Foo, 0), {Builder->getInt32Ty()}, "Bar");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *result = Builder->CreateAdd(Arg1, Builder->getInt32(10), "result");

  Value *fooBar = Builder->CreateAlloca(Foo, nullptr, "fooBar");
  Value *b = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(1)}, "b");

  Value *c = Builder->CreateGEP(Foo, fooBar, {Builder->getInt32(0), Builder->getInt32(2)}, "c");
  Builder->CreateStore(ConstantFP::get(Builder->getDoubleTy(), 3.14), c);

  Builder->CreateRet(fooBar);
  verifyFunction(*fooFunc);

  TheModule->print(errs(), nullptr);
  return 0;
}