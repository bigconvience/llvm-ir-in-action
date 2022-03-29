#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
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

int main(int argc, char *argv[]) {
  InitializeModule();
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, "Foo");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = BasicBlock::Create(*TheContext, "entry", fooFunc);
  Builder->SetInsertPoint(entry);
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;
  Value *c = Builder->CreateAdd(Arg1, Arg2, "c");
  Value *result = Builder->CreateAdd(c, Builder->getInt32(10), "result");
  Builder->CreateRet(result);
  verifyFunction(*fooFunc);

  TheModule->print(errs(), nullptr);
  return 0;
}

