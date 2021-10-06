// Original author: MD Gaziur Rahman Noor

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef bool
#define bool int
#endif
#ifndef True
#define True 1
#endif
#ifndef False
#define False 0
#endif

/*
 * Color hashes for faster lookup.
 * Author: Gabriel Gazola Milan
 *
 * Replaces a bunch of if statements in the code.
 */
#define HASH_BLACK 210668624802U
#define HASH_RED 193468640U
#define HASH_GREEN 210674774454U
#define HASH_YELLOW 6952956836385U
#define HASH_BLUE 6383898381U
#define HASH_MAGENTA 229431915261218U
#define HASH_CYAN 6383947824U
#define HASH_WHITE 210693394662U
#define HASH_BRIGHT_BLACK 13260719720051023329U
#define HASH_BRIGHT_RED 8244608208946659167U
#define HASH_BRIGHT_GREEN 13260719720057172981U
#define HASH_BRIGHT_YELLOW 13328637067256300608U
#define HASH_BRIGHT_BLUE 13817653863305463148U
#define HASH_BRIGHT_MAGENTA 15569909508477893409U
#define HASH_BRIGHT_CYAN 13817653863305512591U
#define HASH_BRIGHT_WHITE 13260719720075793189U

char *inputString() {
  int bufsize = 100;
  int position = 0;
  char *buffer = (char *)malloc(sizeof(char) * bufsize);
  int c;

  while (True) {

    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }

    position++;

    if (position >= bufsize) {
      bufsize += 100;
      buffer = (char *)realloc(buffer, bufsize);
    }
  }

  return buffer;
}
int64_t myFloor(double x) {
  long double xcopy = x < 0 ? x * -1 : x;
  unsigned int zeros = 0;
  long double n = 1;
  for (n = 1; xcopy > n * 10; n *= 10, ++zeros)
    ;
  for (xcopy -= n; zeros != -1; xcopy -= n)
    if (xcopy < 0) {
      xcopy += n;
      n /= 10;
      --zeros;
    }
  xcopy += n;
  return x < 0 ? (xcopy == 0 ? x : x - (1 - xcopy)) : (x - xcopy);
}
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

  if (fmt_size > 0) {
    result = (char *)malloc(sizeof(char));

    // add one character if it's not possibly format string
    if (*fmt != '{' || !(*(fmt + 1) && *(fmt + 1) == '}')) {
      result[0] = *fmt;
      size += 1;
      fmt++;
    }
  } else {
    return (char *)"";
  }

  while (*fmt) {
    // check if it's format string
    if (*fmt == '{') {
      if (*(fmt + 1) && *(fmt + 1) == '}') {
        char *arg = va_arg(args, char *);
        size_t a_size = strlen(arg);

        result = (char *)realloc(result, size + a_size);
        memcpy(result + size, arg, a_size);
        size += a_size;
        fmt++; // ignore '}'
      }
    } else {
      // increase size and push char
      result = (char *)realloc(result, size + 1);
      memcpy(result + size, fmt, 1);
      size += 1;
    }

    fmt++;
  }

  // null terminate if it's not null
  if (result) {
    result = (char *)realloc(result, size + 1);
    result[size] = '\0';
  }

  return result;
}

/* _colorprint() implementation */

/*
 * int64_t
 *
 * Put foreground color in first half
 */

// Foreground(text) colors

/*
 * Hash function for faster color matching
 * Extracted from: https://stackoverflow.com/a/7666577/9944075
 * Added here by: Gabriel Gazola Milan
 * 
 * Implements a hash function for the color codes.
 * 
 * Example:
 * unsigned long hash = _hash("BLACK");
 * 
 * Output:
 * hash = 210668624802
 */
unsigned long _hash(const char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

/*
 * Builtin Color Print Function
 * Author: MD Gaziur Rahman Noor
 *
 * Formats given string specifically so that it is colored as
 * specified when printed to stdout
 *
 * Example:
 * _colorprint("Hello World!", SH_FG_BLACK , true);
 */
void _colorprint(const char *str, char *color, char *bg) {
  int16_t bg_flags;
  int16_t flags;
  if (color)
    switch (_hash(color)) {
      case HASH_BLACK:
        flags = (30 << 8);
        break;
      case HASH_RED:
        flags = (31 << 8);
        break;
      case HASH_GREEN:
        flags = (32 << 8);
        break;
      case HASH_YELLOW:
        flags = (33 << 8);
        break;
      case HASH_BLUE:
        flags = (34 << 8);
        break;
      case HASH_MAGENTA:
        flags = (35 << 8);
        break;
      case HASH_CYAN:
        flags = (36 << 8);
        break;
      case HASH_WHITE:
        flags = (37 << 8);
        break;
      default:
        printf("\033[0;31m%s-> Unknown font color\033[0m", color);
        exit(1);
        break;
    }
  if (bg)
    switch(_hash(bg)) {
      case HASH_BLACK:
        bg_flags = 40;
        break;
      case HASH_RED:
        bg_flags = 41;
        break;
      case HASH_GREEN:
        bg_flags = 42;
        break;
      case HASH_YELLOW:
        bg_flags = 43;
        break;
      case HASH_BLUE:
        bg_flags = 44;
        break;
      case HASH_MAGENTA:
        bg_flags = 45;
        break;
      case HASH_CYAN:
        bg_flags = 46;
        break;
      case HASH_WHITE:
        bg_flags = 47;
        break;
      case HASH_BRIGHT_BLACK:
        bg_flags = 100;
        break;
      case HASH_BRIGHT_RED:
        bg_flags = 101;
        break;
      case HASH_BRIGHT_GREEN:
        bg_flags = 102;
        break;
      case HASH_BRIGHT_YELLOW:
        bg_flags = 103;
        break;
      case HASH_BRIGHT_BLUE:
        bg_flags = 104;
        break;
      case HASH_BRIGHT_MAGENTA:
        bg_flags = 105;
        break;
      case HASH_BRIGHT_CYAN:
        bg_flags = 106;
        break;
      case HASH_BRIGHT_WHITE:
        bg_flags = 107;
        break;
      default:
        printf("\033[0;31m%s-> Unknown background color\033[0m", color);
        exit(1);
        break;
    }
  int8_t foreground = (flags & 0xFF00) >> 8;
  int8_t background = bg_flags & 0x00FF;
  if (background == 0) {
    printf("\e[1;%dm%s", foreground, str);
  } else {
    printf("\e[1;%dm\e[1;%dm%s", foreground, background, str);
  }
  printf("\e[1;0m");
}
double mypow(double base, double power) {
  double result = 1;
  if (power >= 0) {

    while (power--) {
      result = result * base;
    }

    return result;
  } else if (power < 0) {
    while (power++) {
      result = result * base;
    }
    return 1 / result;
  }
}
int compare(char *a, char *b) {
  int flag = 0;
  if (a == NULL || b == NULL) {
    if (a == NULL && b == NULL) {
      flag = 0;
    } else {
      flag = 1;
    }
  } else {
    while (*a != '\0' && *b != '\0') // while loop
    {
      if (*a != *b) {
        flag = 1;
      }
      a++;
      b++;
    }
  }
  return flag;
}
