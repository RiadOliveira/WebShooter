#ifndef CWD_GETTER_H
#define CWD_GETTER_H

#include <limits.h>
#include <stdbool.h>
#include <unistd.h>

#include "errorThrower.h"

#if defined(__linux__)
  #include <linux/limits.h>
#endif

void getCwdOrExit(char* cwd, size_t size);

#endif