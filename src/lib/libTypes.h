#ifndef LIB_TYPES_H
#define LIB_TYPES_H

#include <stdlib.h>

#include "shortcutTypes.h"

typedef enum { C_FILE, C_FOLDER } ContentType;

typedef struct {
  const char* cwd;
  const char** contentOrArchivePaths;
  size_t contentsOrArchivesSize;
} WstParams;

#endif