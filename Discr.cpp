namespace {
  struct DiscrPass : public ModulePass {
    static char ID;
    DbgmapPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &F) {
      ModulePassManager MPM;
      MPM.addPass(createModuleToFunctionPassAdaptor(AddDiscriminatorsPass()));
      MPM.run();
    }
  }
}

char DiscrPass::ID = 0;

static RegisterPass<DiscrPass>
    X("discr", "Discr Pass",
      true, // This pass doesn't modify the CFG => true
      false // This pass is not a pure analysis pass => false
    );
