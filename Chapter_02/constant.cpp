#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <vector>
#include <iostream>
using namespace llvm;

static LLVMContext TheContext;
static LLVMContext& getGlobalContext() {
    return TheContext;
}
static LLVMContext &Context = getGlobalContext();
static Module *ModuleOb = new Module("my compiler", Context);

Function *createFunc(IRBuilder<> &Builder, std::string Name) {
  FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
  Function *fooFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, Name, ModuleOb);
  return fooFunc;
}

BasicBlock *createBB(Function *fooFunc, std::string Name) {
  return BasicBlock::Create(Context, Name, fooFunc);
}

GlobalVariable *createGlob(IRBuilder<> &Builder, Type *type, std::string Name) {
    ModuleOb->getOrInsertGlobal(Name, type); 
    GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
    gVar->setConstant(true);
    gVar->setLinkage(GlobalValue::CommonLinkage);
    gVar->setAlignment(MaybeAlign(4));
    return gVar;
}

int main(int argc, char *argv[]) {
  static IRBuilder<> Builder(Context);
  GlobalVariable *gVar = createGlob(Builder, Builder.getInt32Ty(), "variable");
  gVar->setInitializer(Builder.getInt32(21));
  Function *fooFunc = createFunc(Builder, "foo");
  BasicBlock *entry = createBB(fooFunc, "entry");
  Builder.SetInsertPoint(entry);
    
  StructType *foo = StructType::create(getGlobalContext(), "foo");
  foo->setBody({Builder.getInt32Ty(), Builder.getInt8Ty()});
  GlobalVariable *gfoo = createGlob(Builder, foo, "foo_struct");
  Constant *foo_constant = ConstantStruct::get(foo, 
      {Builder.getInt32(16), Builder.getInt32(4)}); 
  gfoo->setInitializer(foo_constant);

  StructType *bar = StructType::create(getGlobalContext(), "Bar");
  bar->setBody({Builder.getInt8Ty(), Builder.getInt32Ty(), Builder.getInt8PtrTy()});
  ConstantPointerNull *bar_null = ConstantPointerNull::get(PointerType::get(bar, 0));
  Value *next = Builder.CreateGEP(bar_null, {Builder.getInt32(1)}, "next");
  Value *ptr_int = Builder.CreatePtrToInt(next, Builder.getInt32Ty(), "struct_size");
  ptr_int->dump();
  ptr_int->getType()->dump();

  Value *cnt = Builder.getInt32(9);
  
  Value *load = Builder.CreateLoad(Builder.getInt32Ty(), gVar, "var");
   
  Value *stack = Builder.CreateAlloca(Builder.getInt32Ty(), nullptr, "stack");
  Builder.CreateStore(ptr_int, stack);

  Value *length = Builder.CreateAdd(load, Builder.CreateLoad(Builder.getInt32Ty(), stack), "retsult");

  Builder.CreateGlobalString(StringRef("Hello, world!"), "hello");
  Value *ret = Builder.CreateAdd(cnt, length);
  Builder.CreateRet(ret);
  ModuleOb->print(errs(), nullptr);
  return 0;
}
