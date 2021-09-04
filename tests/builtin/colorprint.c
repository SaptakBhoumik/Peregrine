// Original Author: MD Gaziur Rahman Noor
#include "../../swallow/builtin.h"

int main() {
    _colorprintln("Hello World!", SH_BG_RED);
    _colorprintln("Text Color Red", SH_FG_RED);
    _colorprintln("Black text on white bg", SH_FG_BLACK | SH_BG_WHITE);

    _colorprint("This ", SH_FG_GREEN, True);
    _colorprint("is ", SH_FG_BLUE, True);
    _colorprintln("colorprintln()", SH_FG_BRIGHT_MAGENTA);
    return 0;
}