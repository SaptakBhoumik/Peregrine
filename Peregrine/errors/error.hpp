#ifndef PEREGRINE_ERROR
#define PEREGRINE_ERROR
#include <string>
typedef struct {
  unsigned long int line;
  unsigned long int col;
  std::string file;
  std::string code;
} Location;

typedef struct {
  Location loc;
  std::string msg;
  std::string submsg;
  std::string hint;
  std::string ecode;
} PEError;

void display(PEError e);
#endif // PEREGRINE_ERROR