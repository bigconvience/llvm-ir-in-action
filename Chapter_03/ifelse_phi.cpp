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

Function *createMaxProto(std::string funcName) {
  Function *fooFunc = createFunc(Builder->getInt32Ty(), {Builder->getInt32Ty(), Builder->getInt32Ty()}, funcName);
  std::vector<std::string> FuncArgs;
  FuncArgs.push_back("a");
  FuncArgs.push_back("b");
  setFuncArgs(fooFunc, FuncArgs);
  return fooFunc;
}


void createMax_phi() {
  Function *fooFunc = createMaxProto("max_phi");

  // args
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *Arg1 = AI++;
  Value *Arg2 = AI;

  // entry
  BasicBlock *entry = createBB(fooFunc, "entry");
  Builder->SetInsertPoint(entry);
  Value *retVal = Builder->CreateAlloca(Builder->getInt32Ty(), nullptr, "retVal");

  // if condition
  Value *Compare = Builder->CreateICmpULT(Arg1, Arg2, "cmptmp");
  Value *Cond = Builder->CreateICmpNE(Compare, Builder->getInt1(false), "ifcond");

  BasicBlock *ThenBB = createBB(fooFunc, "then");
  BasicBlock *ElseBB = createBB(fooFunc, "else");
  BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
  Builder->CreateCondBr(Cond, ThenBB, ElseBB);

  // Then 
  Builder->SetInsertPoint(ThenBB);
  Value *ThenVal = Builder->CreateAdd(Arg1, Builder->getInt32(1), "thenVal");
  Builder->CreateBr(MergeBB);

  // else
  Builder->SetInsertPoint(ElseBB);
  Value *ElseVal = Builder->CreateMul(Arg2, Builder->getInt32(16), "elseVal");
  Builder->CreateBr(MergeBB);

  // end
  Builder->SetInsertPoint(MergeBB);
  PHINode *Phi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "iftmp");
  Phi->addIncoming(ThenVal, ThenBB);
  Phi->addIncoming(ElseVal, ElseBB);

  Builder->CreateRet(Phi);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  createMax_phi();

  TheModule->print(errs(), nullptr);
  return 0;
}

