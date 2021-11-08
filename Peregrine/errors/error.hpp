#ifndef PEREGRINE_ERROR_HPP
#define PEREGRINE_ERROR_HPP

#include <string>

struct Location {
    size_t line;
    size_t col;
    std::string file;
    std::string code;
};

struct PEError {
    Location loc;
    std::string msg;
    std::string submsg;
    std::string hint;
    std::string ecode;
};

void display(PEError e);

#endif