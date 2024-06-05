#ifndef VALIDATION_H
#define VALIDATION_H

#include <limits.h>

#include "cliTypes.h"
#include "errorPrinting.h"

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

bool validateArguments(ParsedArguments* arguments);
void printHelpMenu();

#endif