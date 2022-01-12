#ifndef PEREGRINE_ERROR_HPP
#define PEREGRINE_ERROR_HPP

#include <string>

const std::string prefix = "\e[";
const std::string suffix = "m";
const std::string reset = prefix + "0" + suffix;

const std::string cyan = "36";
const std::string light_red = "91";
const std::string blue = "94";

const std::string bold = "1";

struct Location {
    size_t line;
    size_t col;
    size_t loc;
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

std::string fg(std::string text, std::string color);
std::string style(std::string text, std::string color);

void display(PEError e);

#endif