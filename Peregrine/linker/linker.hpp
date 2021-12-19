#ifndef PEREGRINE_LINKER
#define PEREGRINE_LINKER
#include <iostream>

enum Flavor {
  Invalid,
  Gnu,       // -flavor gnu
  WinLink,   // -flavor link
  Darwin,    // -flavor darwin
  DarwinOld, // -flavor darwinold
  Wasm,      // -flavor wasm
};

class linker{
    private:
    int argc=0;
    const char **argv;
    public:
    void add_arg( const char* arg);
    int link();//start linking
    void set_flavor(Flavor name);
    void auto_flavor();

};

#endif //PEREGRINE_LINKER