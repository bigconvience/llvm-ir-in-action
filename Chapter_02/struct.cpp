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

Function *createFunc(IRBuilder<> &Builder, ArrayRef<Type*> Params, std::string Name) {
  FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), Params, false);
  Function *fooFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, Name, ModuleOb);
  return fooFunc;
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

int main(int argc, char *argv[]) {
  static IRBuilder<> Builder(Context);
  GlobalVariable *gVar = createGlob(Builder, "variable");
  gVar->setInitializer(Builder.getInt32(21));
  StructType *opaque = StructType::create(getGlobalContext(), "opaque");
  Function *barFunc = createFunc(Builder, {opaque}, "bar");
  Function *fooFunc = createFunc(Builder, {}, "foo");
  BasicBlock *entry = createBB(fooFunc, "entry");
  Builder.SetInsertPoint(entry);
  Value *load = Builder.CreateLoad(Type::getInt32Ty(getGlobalContext()), gVar);
  Value *reg = Builder.CreateAdd(load, Builder.getInt32(2), "reg");

  Type *x = Builder.getInt64Ty();
  Type *y = Builder.getDoubleTy();
  StructType *Foo = StructType::create(getGlobalContext(), "Foo");
  Foo->setBody({x, y});
  Value *stack = Builder.CreateAlloca(Foo, nullptr, "stack");

  StructType *FooBar = StructType::create(getGlobalContext(), "FooBar");
  FooBar->setBody({ Foo, Builder.getInt8PtrTy(), PointerType::get(FooBar, 0)});
  Value *fooBar = Builder.CreateAlloca(FooBar, nullptr, "fooBar");

  Value *op = Builder.CreateAlloca(opaque, nullptr, "op");
  
  StructType *Bar = StructType::create(getGlobalContext(), "Bar");
  Bar->setBody({Builder.getInt32Ty(), Builder.getInt8PtrTy(), Builder.getDoubleTy()});
  Value *bar = Builder.CreateAlloca(Bar, nullptr, "bar");
  Value *gep = Builder.CreateGEP(bar, {Builder.getInt32(0), Builder.getInt32(1)});
  
  Value *baz = Builder.CreateAlloca(Bar, Builder.getInt32(100), "baz");
  Value *qux = Builder.CreateGEP(baz, {Builder.getInt32(17), Builder.getInt32(2)});
  Builder.CreateStore(ConstantFP::get(Builder.getDoubleTy(), 0.0), qux);

  ArrayType *arrayType = ArrayType::get(Bar, 100);
  Value *foos = Builder.CreateAlloca(arrayType, nullptr, "foos");
  Value *foo_17 = Builder.CreateGEP(foos, 
      {Builder.getInt32(0), Builder.getInt32(17), Builder.getInt32(2)});
  Builder.CreateStore(ConstantFP::get(Builder.getDoubleTy(), 0.0), foo_17);

  Builder.CreateRet(Builder.getInt32(0));
  
  verifyFunction(*fooFunc);
  ModuleOb->print(errs(), nullptr);
  return 0;
}
