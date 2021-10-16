#ifndef peregrine_errors_hpp
#define peregrine_errors_hpp

#include <string>
#include <iostream>
#include <map>

using namespace std;

string prefix = "\e[";
string suffix = "m";
string reset = prefix + "0" + suffix;

string cyan = "36";
string light_red = "91";
string blue = "94";

string bold = "1";

string fg(string text, string color) {
    return prefix + color + suffix + text + reset;
}

string style(string text, string color) {
    return prefix + color + suffix + text + reset;
}

typedef struct Location {
    int line;
    int col;
    string file;
    string code;
} Location;

typedef struct PEError {
    Location loc;
    string msg;
    string submsg;
    string hint;
    string ecode;
} PEError;

void display(PEError e) {
    cout << "  ╭- " << fg(style("Error ---------------------------------------- " + e.loc.file + ":" + to_string(e.loc.line) + ":" + to_string(e.loc.col), bold), light_red) << endl;
    cout << "  | " << fg(style(e.msg, bold), light_red) << endl;
    cout << "  |" << endl;
    cout << "  |" << endl;
    cout << "  |" << endl;
    cout << to_string(e.loc.line) << " | " << e.loc.code.substr(0, e.loc.code.length()) << endl;
    cout << "  |";
    cout << string(e.loc.col, ' ');
    cout << fg("~", light_red);
    cout << fg(style(" <----- " + e.submsg, bold), light_red) << endl;
    cout << "  |\n  |\n";
    if (e.hint != "") {
        cout << "  ├- " << fg(style("Try: ", bold), blue) << e.hint << endl;
        cout << "  |" << endl;
    }
    cout << "  ╰- " << fg(style("Hint: ", bold), cyan) << "Use peregrine --explain=" << e.ecode << endl;

    exit(-1);
}

#endif //peregrine_errors_hpp
