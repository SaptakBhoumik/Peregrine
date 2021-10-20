
#include "error.hpp"
#include <iostream>
#include <map>
#include <string>

std::string prefix = "\e[";
std::string suffix = "m";
std::string reset = prefix + "0" + suffix;

std::string cyan = "36";
std::string light_red = "91";
std::string blue = "94";

std::string bold = "1";

std::string fg(std::string text, std::string color) {
  return prefix + color + suffix + text + reset;
}

std::string style(std::string text, std::string color) {
  return prefix + color + suffix + text + reset;
}

void display(PEError e) {
  std::cout << "  ╭- "
            << fg(style("Error ---------------------------------------- " +
                            e.loc.file + ":" + std::to_string(e.loc.line) +
                            ":" + std::to_string(e.loc.col),
                        bold),
                  light_red)
            << std::endl;
  std::cout << "  | " << fg(style(e.msg, bold), light_red) << std::endl;
  std::cout << "  |" << std::endl;
  std::cout << "  |" << std::endl;
  std::cout << "  |" << std::endl;
  std::cout << std::to_string(e.loc.line) << " | "
            << e.loc.code.substr(0, e.loc.code.length()) << std::endl;
  std::cout << "  |";
  std::cout << std::string(e.loc.col, ' ');
  std::cout << fg("~", light_red);
  std::cout << fg(style(" <----- " + e.submsg, bold), light_red) << std::endl;
  std::cout << "  |\n  |\n";
  if (e.hint != "") {
    std::cout << "  ├- " << fg(style("Try: ", bold), blue) << e.hint
              << std::endl;
    std::cout << "  |" << std::endl;
  }
  std::cout << "  ╰- " << fg(style("Hint: ", bold), cyan)
            << "Use peregrine --explain=" << e.ecode << std::endl;

}
