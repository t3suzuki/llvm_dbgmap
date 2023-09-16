#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  int newLine = 77777;
  struct DbgmapPass : public FunctionPass {
    static char ID;
    DbgmapPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      
      for (auto &BB : F) {
        for (auto &I : BB) {
	  auto loc = I.getDebugLoc();
	  DILocation *dil = loc.get();
	  //loc.dump();
	  //errs() << "\n";
	  if (dil) {
	    DILocation *newdil = DILocation::get(F.getContext(), newLine++, 0, dil->getScope());

	    auto newloc = new DebugLoc(newdil);
	    I.setDebugLoc(*newloc);
	  }
	  //newloc->dump();
	  //errs() << "\n";	  
	}
      }
      return false;
    }
  };
}

char DbgmapPass::ID = 0;

#if 0
// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerDbgmapPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new DbgmapPass());
}

static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerDbgmapPass);
#endif

static RegisterPass<DbgmapPass>
    X("hello-world", "Hello World Pass",
      true, // This pass doesn't modify the CFG => true
      false // This pass is not a pure analysis pass => false
    );
