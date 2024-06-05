#ifndef LIB_TYPES_H
#define LIB_TYPES_H

#include <stdlib.h>

#include "shortcutTypes.h"

typedef struct {
  const char* cwd;
  const char** archiveOrContentPaths;
  size_t archivesOrContentsSize;
} WstOperationParams;

#endif