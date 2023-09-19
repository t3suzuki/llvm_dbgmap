#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  int newDisc = 77;
  using Location = std::pair<StringRef, unsigned>;
  using LocationDiscriminatorMap = DenseMap<Location, unsigned>;
  struct DbgmapPass : public FunctionPass {
    static char ID;
    DbgmapPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      
      LocationDiscriminatorMap LDM;
      for (auto &BB : F) {
        for (auto &I : BB) {
	  if (1) {
	    const DILocation *DIL = I.getDebugLoc();
	    if (DIL) {
#if 1
	      auto Discriminator = ++newDisc;
	      auto NewDIL = DIL->cloneWithBaseDiscriminator(Discriminator);
	      if (NewDIL != std::nullopt) {
		I.setDebugLoc(*NewDIL);
	      } else {
		assert(0);
	      } 
#else
	      Location L = std::make_pair(DIL->getFilename(), DIL->getLine());
	      auto Discriminator = ++LDM[L];
	      auto NewDIL = DIL->cloneWithBaseDiscriminator(Discriminator);
	      if (NewDIL != std::nullopt) {
		I.setDebugLoc(*NewDIL);
	      } else {
		assert(0);
	      }
#endif
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
