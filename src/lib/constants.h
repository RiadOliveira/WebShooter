#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PATH_MAX_SIZE 4096
#define CONTENT_NAME_MAX_SIZE 256

#define NULL_TERMINATOR '\0'
#define DOT_CHAR '.'

#if defined(_WIN32) || defined(_WIN64)
  #define PATH_SEPARATOR '\\'
#else
  #define PATH_SEPARATOR '/'
#endif

#endif