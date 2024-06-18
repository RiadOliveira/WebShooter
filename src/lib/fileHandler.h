#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdbool.h>
#include <string.h>

#include "constants.h"
#include "errorThrower.h"

#define READ_BINARY_MODE "rb"
#define WRITE_BINARY_MODE "wb"

FILE* openFileOrExit(const char* path, const char* modes);

void appendPath(char* destination, unsigned int position, const char* source);

#endif