#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
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

BasicBlock *createBB(Function *fooFunc, std::string Name) {
  return BasicBlock::Create(*TheContext, Name, fooFunc);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, "max");
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);

  BasicBlock *entry = createBB(fooFunc, "entry");
  Builder->SetInsertPoint(entry);
  Value *retVal = Builder->CreateAlloca(Builder->getInt32Ty(), nullptr, "retVal");
  
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;

  // if condition
  Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
  Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");

  BasicBlock *ThenBB = createBB(fooFunc, "then");
  BasicBlock *ElseBB = createBB(fooFunc, "else");
  BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
  Builder->CreateCondBr(Cond, ThenBB, ElseBB);

  // Then 
  Builder->SetInsertPoint(ThenBB);
  Builder->CreateStore(Arg1, retVal);
  Builder->CreateBr(MergeBB);

  // else
  Builder->SetInsertPoint(ElseBB);
  Builder->CreateStore(Arg2, retVal);
  Builder->CreateBr(MergeBB);

  // end
  Builder->SetInsertPoint(MergeBB);
  Value *maxVal = Builder->CreateLoad(Builder->getInt32Ty(), retVal);
  Builder->CreateRet(maxVal);

  TheModule->print(errs(), nullptr);
  return 0;
}

