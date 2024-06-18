#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include "constants.h"
#include "errorThrower.h"

#if defined(_WIN32) || defined(_WIN64)
  #define stat _stat
#endif

#define READ_BINARY_MODE "rb"
#define WRITE_BINARY_MODE "wb"

FILE* openFileOrExit(const char* path, const char* modes);
DIR* openFolderOrExit(const char* path);

void createFolder(const char* path);
void appendPath(char* destination, unsigned int position, const char* source);

#endif