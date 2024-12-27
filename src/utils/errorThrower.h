#ifndef ERROR_THROWER
#define ERROR_THROWER

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exitWithMessage(const char* format, ...);

#endif