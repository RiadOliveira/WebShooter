#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

#include "constants.h"
#include "errorThrower.h"

FILE* openFileOrExit(const char* path, const char* modes);

#endif