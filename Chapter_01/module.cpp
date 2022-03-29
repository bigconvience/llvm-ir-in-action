#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first module", *TheContext);
}

int main(int argc, char *argv[]) {
  InitializeModule();
  TheModule->print(errs(), nullptr);
  return 0;
}

