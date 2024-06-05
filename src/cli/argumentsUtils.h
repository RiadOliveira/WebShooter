#ifndef ARGUMENTS_UTILS_H
#define ARGUMENTS_UTILS_H

#include <limits.h>

#include "argumentsParsing.h"
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

bool printErrorIfInvalidArguments(ParsedArguments* arguments);
void printHelpMenu();

char* getCwdOrPrintError();
void freeArguments(ParsedArguments* arguments);

#endif