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

    if (!fmt || strlen(fmt) == 0) {
        return "";
    }

    char *result = NULL;
    size_t space = 0;

    while(*fmt != '\0') {
        if (*fmt == '{' && *(fmt + 1) == '}') {
            char *arg = va_arg(args, char*);

            // allocate if doesn't exist
            if (!result) {
                result = (char*)malloc(sizeof(char) * strlen(arg));
                space = strlen(arg);
                // memcpy
                memcpy(result, arg, strlen(arg));
            }
            else {
                // reallocate
                result = (char*)realloc(result, space + strlen(arg));

                // memcpy
                memcpy(result + space, arg, strlen(arg));
                space += strlen(arg);
            }
            fmt++;
        } else {
            // allocate if isn't allocated already
            if(!result) {
                result = (char*)malloc(sizeof(char));
                memcpy(result + space, fmt, 1);
                space = 1;
            } else {
                result = (char*)realloc(result, space + 1);
                memcpy(result + space, fmt, 1);
                space += 1;
            }
        }
        fmt++;
    }
    result[space] = '\0';

    return result;
}

#endif //SWALLOW_BUILTIN_H