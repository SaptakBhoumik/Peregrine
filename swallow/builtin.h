// Original author: MD Gaziur Rahman Noor

#include <stdarg.h>

#include <stdlib.h>

#include <string.h>

#include <stdint.h>

#include <stdio.h>

#ifndef bool
#define bool int
#endif
#ifndef True
#define True 1
#endif
#ifndef False
#define False 0
#endif

char * inputString() {
  int bufsize = 100;
  int position = 0;
  char * buffer = (char * ) malloc(sizeof(char) * bufsize);
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
      buffer = (char * ) realloc(buffer, bufsize);
    }
  }

  return buffer;
}
int64_t myFloor(double x) {
  long double xcopy = x < 0 ? x * -1 : x;
  unsigned int zeros = 0;
  long double n = 1;
  for (n = 1; xcopy > n * 10; n *= 10, ++zeros);
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
char * _format(const char * fmt, ...) {
  va_list args;
  va_start(args, fmt);

  // make sure it's not empty or null
  char * result = NULL;
  size_t size = 0;
  size_t fmt_size = strlen(fmt);

  if (fmt_size > 0) {
    result = (char * ) malloc(sizeof(char));

    // add one character if it's not possibly format string
    if ( * fmt != '{' || !( * (fmt + 1) && * (fmt + 1) == '}')) {
      result[0] = * fmt;
      size += 1;
      fmt++;
    }
  } else {
    return (char * )
    "";
  }

  while ( * fmt) {
    // check if it's format string
    if ( * fmt == '{') {
      if ( * (fmt + 1) && * (fmt + 1) == '}') {
        char * arg = va_arg(args, char * );
        size_t a_size = strlen(arg);

        result = (char * ) realloc(result, size + a_size);
        memcpy(result + size, arg, a_size);
        size += a_size;
        fmt++; // ignore '}'
      }
    } else {
      // increase size and push char
      result = (char * ) realloc(result, size + 1);
      memcpy(result + size, fmt, 1);
      size += 1;
    }

    fmt++;
  }

  // null terminate if it's not null
  if (result) {
    result = (char * ) realloc(result, size + 1);
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
 * Builtin Color Print Function
 * Author: MD Gaziur Rahman Noor
 *
 * Formats given string specifically so that it is colored as
 * specified when printed to stdout
 *
 * Example:
 * _colorprint("Hello World!", SH_FG_BLACK , true);
 */
void _colorprint(const char * str, char * color, char * bg) {
  int16_t bg_flags;
  int16_t flags;
  if (strcmp(color, "BLACK") == 0) {
    flags = (30 << 8);
  } else if (strcmp(color, "RED") == 0) {
    flags = (31 << 8);
  } else if (strcmp(color, "GREEN") == 0) {
    flags = (32 << 8);
  } else if (strcmp(color, "YELLOW") == 0) {
    flags = (33 << 8);
  } else if (strcmp(color, "BLUE") == 0) {
    flags = (34 << 8);
  } else if (strcmp(color, "MAGENTA") == 0) {
    flags = (35 << 8);
  } else if (strcmp(color, "CYAN") == 0) {
    flags = (36 << 8);
  } else if (strcmp(color, "WHITE") == 0) {
    flags = (37 << 8);
  } else {
    printf("\033[0;31m%s-> Unknown font color\033[0m", color);
    exit(1);
  }
  if (bg != NULL) {
    if (strcmp(bg, "BLACK") == 0) {
      bg_flags = 40;
    } else if (strcmp(bg, "RED") == 0) {
      bg_flags = 41;
    } else if (strcmp(bg, "GREEN") == 0) {
      bg_flags = 42;
    } else if (strcmp(color, "YELLOW") == 0) {
      bg_flags = 43;
    } else if (strcmp(bg, "BLUE") == 0) {
      bg_flags = 44;
    } else if (strcmp(bg, "MAGENTA") == 0) {
      bg_flags = 45;
    } else if (strcmp(bg, "CYAN") == 0) {
      bg_flags = 46;
    } else if (strcmp(bg, "WHITE") == 0) {
      bg_flags = 47;
    } else if (strcmp(bg, "BRIGHT_BLACK") == 0) {
      bg_flags = 100;
    } else if (strcmp(bg, "BRIGHT_RED") == 0) {
      bg_flags = 101;
    } else if (strcmp(bg, "BRIGHT_GREEN") == 0) {
      bg_flags = 102;
    } else if (strcmp(color, "BRIGHT_YELLOW") == 0) {
      bg_flags = 103;
    } else if (strcmp(bg, "BRIGHT_BLUE") == 0) {
      bg_flags = 104;
    } else if (strcmp(bg, "BRIGHT_MAGENTA") == 0) {
      bg_flags = 105;
    } else if (strcmp(bg, "BRIGHT_CYAN") == 0) {
      bg_flags = 106;
    } else if (strcmp(bg, "BRIGHT_WHITE") == 0) {
      bg_flags = 107;
    } else {
      printf("\033[0;31m%s-> Unknown background color\033[0m", color);
      exit(1);
    }
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
int compare(char * a, char * b) {
  int flag = 0;
  if (a == NULL || b == NULL) {
    if (a == NULL && b == NULL) {
      flag = 0;
    } else {
      flag = 1;
    }
  } else {
    while ( * a != '\0' && * b != '\0') // while loop  
    {
      if ( * a != * b) {
        flag = 1;
      }
      a++;
      b++;
    }
  }
  return flag;
}