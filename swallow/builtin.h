// Original author: MD Gaziur Rahman Noor

#ifndef SWALLOW_BUILTIN_H
#define SWALLOW_BUILTIN_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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
        result = malloc(sizeof(char));

        // add one character if it's not possibly format string
        if (*fmt != '{' || !(*(fmt+1) && *(fmt+1) == '}')) {
            result[0] = *fmt;
            size += 1;
            fmt++;
        }
    } else {
        return "";
    }

    while(*fmt) {
        // check if it's format string
        if (*fmt == '{') {
            if (*(fmt+1) && *(fmt+1) == '}') {
                char *arg = va_arg(args, char*);
                size_t a_size = strlen(arg);

                result = realloc(result, size + a_size);
                memcpy(result + size, arg, a_size);
                size += a_size;
                fmt++; // ignore '}'
            }
        }
        else {
            // increase size and push char
            result = realloc(result, size + 1);
            memcpy(result + size, fmt, 1);
            size += 1;
        }

        fmt++;
    }

    // null terminate if it's not null
    if(result) {
        result = realloc(result, size + 1);
        result[size] = '\0';
    }

    return result;
}
#endif //SWALLOW_BUILTIN_H