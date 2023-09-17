#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  int newDisc = 777777;
  struct DbgmapPass : public FunctionPass {
    static char ID;
    DbgmapPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      
      for (auto &BB : F) {
        for (auto &I : BB) {
	  const DILocation *DIL = I.getDebugLoc();
	  if (DIL) {
	    auto Discriminator = ++newDisc;
	    const DILocation *NewDIL = DIL->cloneWithDiscriminator(Discriminator);
	    if (NewDIL) {
	      auto NewLoc = new DebugLoc(NewDIL);
	      I.setDebugLoc(*NewLoc);
	    }
	  }
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
    X("dbgmap", "Dbgmap Pass",
      true, // This pass doesn't modify the CFG => true
      false // This pass is not a pure analysis pass => false
    );
