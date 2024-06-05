#ifndef CWD_GETTER_H
#define CWD_GETTER_H

#include <limits.h>
#include <stdbool.h>

#include "errorThrower.h"

#if defined(_WIN32) || defined(_WIN64)
  #include <direct.h>
  #define getCWDBasedOnSO _getcwd
#else
  #if defined(__linux__)
    #include <linux/limits.h>
  #endif

  #include <unistd.h>
  #define getCWDBasedOnSO getcwd
#endif

void getCwdOrExit(char* cwd, size_t size);

#endif