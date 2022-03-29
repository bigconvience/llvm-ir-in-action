#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first module", *TheContext);
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

void createSum() {
  Function *fooFunc = createMaxProto("sum");

  // args
  Function::arg_iterator AI = fooFunc->arg_begin();
  Value *StartVal = AI++;
  Value *EndVal = AI;

  BasicBlock *entryBB = createBB(fooFunc, "entry");
  BasicBlock *loopBB = createBB(fooFunc, "loop");
  BasicBlock *endEntryBB = createBB(fooFunc, "endEntry");
  BasicBlock *endLoopBB = createBB(fooFunc, "endLoop");

  // entry
  Builder->SetInsertPoint(entryBB);
  Value *initVal = Builder->CreateSub(StartVal, EndVal, "init");
  Value *EndCond = Builder->CreateICmpULE(StartVal, EndVal, "entryEndCond");
  EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "entryCond");
  Builder->CreateCondBr(EndCond, loopBB, endEntryBB);

  // loop 
  Builder->SetInsertPoint(loopBB);

  PHINode *iPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "i");
  iPhi->addIncoming(StartVal, entryBB);
  PHINode *sumPhi = Builder->CreatePHI(Builder->getInt32Ty(), 2, "sum");
  
  sumPhi->addIncoming(initVal, loopBB);
  Value *nextI = Builder->CreateAdd(iPhi, Builder->getInt32(1), "nextI");
  Value *nextSum = Builder->CreateAdd(sumPhi, iPhi, "nextSum");

  EndCond = Builder->CreateICmpULE(nextI, EndVal, "loopEndCond");
  EndCond = Builder->CreateICmpNE(EndCond, Builder->getInt1(false), "loopCond");
  Builder->CreateCondBr(EndCond, loopBB, endLoopBB);

  iPhi->addIncoming(nextI, loopBB);
  sumPhi->addIncoming(nextSum, loopBB);

  // endLoopBB
  Builder->SetInsertPoint(endLoopBB);
  Builder->CreateRet(sumPhi);

  // endInit
  Builder->SetInsertPoint(endEntryBB);
  Builder->CreateRet(initVal);
}


int main(int argc, char *argv[]) {
  InitializeModule();

  createSum();

  TheModule->print(outs(), nullptr);
  return 0;
}

