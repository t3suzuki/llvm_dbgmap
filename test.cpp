#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/AddDiscriminators.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main(int argc, char **argv) {
  cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<input llvm file>"), cl::Required);
  cl::opt<std::string> OutputFilename(cl::Positional, cl::desc("<output llvm file>"), cl::Required);

  cl::ParseCommandLineOptions(argc, argv);

  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
  if (!M) {
    Err.print(argv[0], errs());
    return 1;
  }
  ModulePassManager MPM;
  MPM.addPass(createModuleToFunctionPassAdaptor(AddDiscriminatorsPass()));
  ModuleAnalysisManager MAM;
  MPM.run(*M, MAM);
#if 0
  std::error_code EC;
  raw_fd_ostream OS(OutputFilename, EC, llvm::sys::fs::OF_None);
  if (EC) {
    errs() << "Could not open file: " << EC.message() << "\n";
    return 1;
  }
  M->print(OS, nullptr);
#endif

  return 0;
}
