#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

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

GlobalVariable *createGlob(Type *type, std::string name) {
    TheModule->getOrInsertGlobal(name, type);
    GlobalVariable *gVar = TheModule->getNamedGlobal(name);
    gVar->setConstant(true);
    return gVar;
}

int main(int argc, char *argv[]) {
  InitializeModule();
  GlobalVariable *gVar = createGlob(Builder->getInt32Ty(), "variable");
  gVar->setInitializer(Builder->getInt32(21));


  StructType *Foo = StructType::create(*TheContext, "Foo");
  Foo->setBody({Builder->getInt32Ty(), Builder->getDoubleTy()});
  GlobalVariable *gfoo = createGlob(Foo, "foo_var");
  Constant *foo_constant = ConstantStruct::get(Foo, 
      {Builder->getInt32(16), ConstantFP::get(Builder->getDoubleTy(), 3.14)}); 
  gfoo->setInitializer(foo_constant);

  TheModule->print(errs(), nullptr);
  return 0;
}

