// Original author: MD Gaziur Rahman Noor

#ifndef SWALLOW_BUILTIN_H
#define SWALLOW_BUILTIN_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define True true
#define False false

/*
 * Builtin formatter function
 * Author: MD Gaziur Rahman Noor
 *
 * Made specifically to work on char* or char[] only.
 * Using any other types might result in a segfault.
 *
 * Example:
 * _format("Hello {}!\n", "world");
 *
 * Output:
 * Hello world!
 *
 */
char *_format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // make sure it's not empty or null
    char *result = NULL;
    size_t size = 0;
    size_t fmt_size = strlen(fmt);

    if(fmt_size > 0) {
        result = (char*)malloc(sizeof(char));

        // add one character if it's not possibly format string
        if (*fmt != '{' || !(*(fmt+1) && *(fmt+1) == '}')) {
            result[0] = *fmt;
            size += 1;
            fmt++;
        }
    } else {
        return (char*)"";
    }

    while(*fmt) {
        // check if it's format string
        if (*fmt == '{') {
            if (*(fmt+1) && *(fmt+1) == '}') {
                char *arg = va_arg(args, char*);
                size_t a_size = strlen(arg);

                result = (char*)realloc(result, size + a_size);
                memcpy(result + size, arg, a_size);
                size += a_size;
                fmt++; // ignore '}'
            }
        }
        else {
            // increase size and push char
            result = (char*)realloc(result, size + 1);
            memcpy(result + size, fmt, 1);
            size += 1;
        }

        fmt++;
    }

    // null terminate if it's not null
    if(result) {
        result = (char*)realloc(result, size + 1);
        result[size] = '\0';
    }

    return result;
}

/* _colorprint() implementation */

/*
 * int16_t
 *
 * Put foreground color in first half
 * and background color in second half
 */

// Foreground(text) colors
#define SH_FG_BLACK             (30 << 8)
#define SH_FG_RED               (31 << 8)
#define SH_FG_GREEN             (32 << 8)
#define SH_FG_YELLOW            (33 << 8)
#define SH_FG_BLUE              (34 << 8)
#define SH_FG_MAGENTA           (35 << 8)
#define SH_FG_CYAN              (36 << 8)
#define SH_FG_WHITE             (37 << 8)
#define SH_FG_BRIGHT_BLACK      (30 << 8)
#define SH_FG_BRIGHT_RED        (31 << 8)
#define SH_FG_BRIGHT_GREEN      (32 << 8)
#define SH_FG_BRIGHT_YELLOW     (33 << 8)
#define SH_FG_BRIGHT_BLUE       (34 << 8)
#define SH_FG_BRIGHT_MAGENTA    (35 << 8)
#define SH_FG_BRIGHT_CYAN       (36 << 8)
#define SH_FG_BRIGHT_WHITE      (37 << 8)

// Background(text) colors
#define SH_BG_BLACK             40
#define SH_BG_RED               41
#define SH_BG_GREEN             42
#define SH_BG_YELLOW            43
#define SH_BG_BLUE              44
#define SH_BG_MAGENTA           45
#define SH_BG_CYAN              46
#define SH_BG_WHITE             47
#define SH_BG_BRIGHT_BLACK      100
#define SH_BG_BRIGHT_RED        101
#define SH_BG_BRIGHT_GREEN      102
#define SH_BG_BRIGHT_YELLOW     103
#define SH_BG_BRIGHT_BLUE       104
#define SH_BG_BRIGHT_MAGENTA    105
#define SH_BG_BRIGHT_CYAN       106
#define SH_BG_BRIGHT_WHITE      107

/*
 * Builtin Color Print Function
 * Author: MD Gaziur Rahman Noor
 *
 * Formats given string specifically so that it is colored as
 * specified when printed to stdout
 *
 * Example:
 * _colorprint("Hello World!", SH_FG_BLACK | SH_BG_WHITE, true);
 */
void _colorprint(const char *str, int16_t flags, bool reset) {
    int8_t background = flags & 0x00FF;
    int8_t foreground = (flags & 0xFF00) >> 8;

    if(background == 0) {
        // don't include  background
        printf("\e[1;%dm%s", foreground, str);
    }
    else {
        printf("\e[1;%dm\e[1;%dm%s", foreground, background, str);
    }

    if(reset)
        printf("\e[1;0m");
}

/*
 * Wrapper around _colorprint function which adds newline at the end
 * Author: MD Gaziur Rahman Noor
 *
 * Works same as _colorprint() but adds newline at the end
 *
 * Example:
 * _colorprintln("Hello World!", SH_FG_BLACK | SH_BG_WHITE);
 */
void _colorprintln(const char *str, int16_t flags) {
    char *formatted_string = _format("{}\e[1;0m\n", str, False);
    _colorprint(formatted_string, flags, False);
    free(formatted_string);
}
#endif //SWALLOW_BUILTIN_H