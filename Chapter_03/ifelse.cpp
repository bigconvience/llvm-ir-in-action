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
static std::vector<std::string> FunArgs;
typedef SmallVector<BasicBlock *, 16> BBList;
typedef SmallVector<Value *, 16> ValList;

Function *createFunc(IRBuilder<> &Builder, std::string Name) {
  std::vector<Type *> Integers(FunArgs.size(), Type::getInt32Ty(Context));
  FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), Integers, false);
  Function *fooFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, Name, ModuleOb);
  return fooFunc;
}

void setFuncArgs(Function *fooFunc, std::vector<std::string> FunArgs) {
    unsigned Idx = 0;
    Function::arg_iterator AI, AE;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE;
         ++AI, ++Idx)
        AI->setName(FunArgs[Idx]);
}

BasicBlock *createBB(Function *fooFunc, std::string Name) {
  return BasicBlock::Create(Context, Name, fooFunc);
}

GlobalVariable *createGlob(IRBuilder<> &Builder, std::string Name) {
    ModuleOb->getOrInsertGlobal(Name, Builder.getInt32Ty());
    GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
    gVar->setLinkage(GlobalValue::CommonLinkage);
    gVar->setAlignment(MaybeAlign(4));
    return gVar;
}

Value *createArith(IRBuilder<> &Builder, Value *L, Value *R) {
  return Builder.CreateMul(L, R, "multmp");
}

Value *createIfElse(IRBuilder<> &Builder, BBList List, ValList VL) {
  Value *Condtn = VL[0];
  Value *Arg1 = VL[1];
  BasicBlock *ThenBB = List[0];
  BasicBlock *ElseBB = List[1];
  BasicBlock *MergeBB = List[2];
  Builder.CreateCondBr(Condtn, ThenBB, ElseBB);

  Builder.SetInsertPoint(ThenBB);
  Value *ThenVal = Builder.CreateAdd(Arg1, Builder.getInt32(1), "thenaddtmp");
  Builder.CreateBr(MergeBB);

  Builder.SetInsertPoint(ElseBB);
  Value *ElseVal = Builder.CreateAdd(Arg1, Builder.getInt32(2), "elseaddtmp");
  Builder.CreateBr(MergeBB);

  unsigned PhiBBSize = List.size() - 1;
  Builder.SetInsertPoint(MergeBB);
  PHINode *Phi = Builder.CreatePHI(Type::getInt32Ty(getGlobalContext()),
                                   PhiBBSize, "iftmp");
  Phi->addIncoming(ThenVal, ThenBB);
  Phi->addIncoming(ElseVal, ElseBB);

  return Phi;
}

int main(int argc, char *argv[]) {
  FunArgs.push_back("a");
  FunArgs.push_back("b");
  static IRBuilder<> Builder(Context);
  GlobalVariable *gVar = createGlob(Builder, "x");
  Function *fooFunc = createFunc(Builder, "foo");
  setFuncArgs(fooFunc, FunArgs);
  BasicBlock *entry = createBB(fooFunc, "entry");
  Builder.SetInsertPoint(entry);
  Value *Arg1 = fooFunc->arg_begin();
  Value *constant = Builder.getInt32(16);
  Value *val = createArith(Builder, Arg1, constant);
    
  Value *val2 = Builder.getInt32(100);
  Value *Compare = Builder.CreateICmpULT(val, val2, "cmptmp");
  Value *Condtn = Builder.CreateICmpNE(Compare, Builder.getInt1(false), "ifcond");

  ValList VL;
  VL.push_back(Condtn);
  VL.push_back(Arg1);

  BasicBlock *ThenBB = createBB(fooFunc, "then");
  BasicBlock *ElseBB = createBB(fooFunc, "else");
  BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
  BBList List;
  List.push_back(ThenBB);
  List.push_back(ElseBB);
  List.push_back(MergeBB);

  Value *v = createIfElse(Builder, List, VL);

  Builder.CreateRet(v);

  verifyFunction(*fooFunc);
  ModuleOb->print(errs(), nullptr);
  return 0;
}