#ifndef ERROR_THROWER_H_
#define ERROR_THROWER_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exitWithMessage(const char* format, ...);

#endif