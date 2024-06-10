#ifndef LIB_TYPES_H
#define LIB_TYPES_H

#include <stdlib.h>

#include "constants.h"
#include "shortcutTypes.h"

typedef struct {
  const char* cwd;
  const char** contentOrArchivePaths;
  size_t contentsOrArchivesQuantity;
} WstParams;

typedef struct ContentData {
  char name[CONTENT_NAME_MAX_SIZE];
  ullong size;
  struct ContentData* subContents;
  size_t subContentsQuantity;
} ContentData;

#endif