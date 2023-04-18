#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "tinylang/Basic/Version.h"

int main(int argc_, const char **argv_) {

    llvm::InitLLVM X(argc_, argv_);
    llvm::outs() << "Hello, I am tinylang "
                 << tinylang::getTinylangVersion()
                 << "\n";

    return 0;
}
