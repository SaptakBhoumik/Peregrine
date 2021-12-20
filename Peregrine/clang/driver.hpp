#ifndef PEREGRINE_CLANG
#define PEREGRINE_CLANG
#include "llvm/ADT/ArrayRef.h" 
int cc1_main(llvm::ArrayRef<const char *> Argv, const char *Argv0,
                    void *MainAddr);
int cc1as_main(llvm::ArrayRef<const char *> Argv, const char *Argv0,
                      void *MainAddr);
int cc1gen_reproducer_main(llvm::ArrayRef<const char *> Argv,
                                  const char *Argv0, void *MainAddr);
class Compiler{
    private:
    int Argc=1;
    const char **Argv;
    public:
    Compiler(const char *Argv0);
    void populate_args();
    void add_arg(const char * arg);
    int compile();
};
#endif