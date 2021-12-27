# LLVM IR in Action:创建Module

标签（空格分隔）： LLVM Module

---
本文介绍在Ubuntu环境下安装LLVM.由于本系列介绍LLVM IR的用法，不涉及Pass的部分，故无需使用源码编译。
## 开发环境配置
### 安装LLVM
操作系统：[ubuntu 20.0.4](https://cn.ubuntu.com/download/desktop)
LLVM安装
```
sudo apt-get install clang
sudo apt-get install llvm
```
查看安装情况
```
clang --version
llvm-config --version
```
默认会安装llvm-10.若要安装最新版本，如llvm-13，则需要[修改apt source](https://apt.llvm.org/)后重新安装llvm.或用命令安装
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 13
```
安装完成后，使用命令查看安装地址
```
whereis clang-13
whereis llvm-config-13
```
可以使用创建软连接的方式，将clang-13设置为默认版本
```
ln -s clang-13 clang
ln -s clang++13 clang++
ln -s llvm-config-13 llvm-config
```

### 常用命令
clang
```
clang hello.c -o a.out
clang -emit-llvm -S hello.c -o hello.ll
```
clang++
```
clang++ hello.cpp -o a.out
clang++ -emit-llvm -S hello.cpp -o hello.ll
```
llvm-as
```
llvm-as hello.ll -o hello.bc
```
llvm-dis
```
llvm-dis hello.bc -o hello.ll
```
llc
```
llc hello.ll -o hello.s
```
lli
```
lli hello.ll
```
更多命令查看[LLVM Command Guide](https://www.llvm.org/docs/CommandGuide/index.html#llvm-command-guide)

## 第一个Module
创建Module需要用到LLVMContext和Module这两个类。
### 引入头文件和命名空间:
```
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;
```
### 创建LLVMContext和Module
```
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
}
```
### 打印Module
```
int main(int argc, char *argv[]) {
  InitializeModule();
  TheModule->print(errs(), nullptr);
  return 0;
}
```

### 运行程序
执行命令
```
clang++ module.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o toy.out
./toy.out
```
输出：
```
; ModuleID = 'first modlue'
source_filename = "first modlue"
```

## 总结
在Ubuntu上使用命令安装LLVM编译好的LLVM非常方便。若要源码安装，参考[Getting Started with the LLVM System](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm).
[LLVM Command Guide](https://www.llvm.org/docs/CommandGuide/index.html#llvm-command-guide)列出了所有LLVM工具的命令行用法.
关于LLVM API的更多用法，查看[Doxygen generated documentation](https://llvm.org/doxygen/index.html)

### 源代码
```
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

static void InitializeModule() {
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("first modlue", *TheContext);
}

int main(int argc, char *argv[]) {
  InitializeModule();
  TheModule->print(errs(), nullptr);
  return 0;
}
```
### Git地址
[LLVM IR in Action/Chapter_01](https://github.com/bigconvience/llvm-ir-in-action)
