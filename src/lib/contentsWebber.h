#ifndef CONTENTS_WEBBER_H
#define CONTENTS_WEBBER_H

#include <dirent.h>

#include "dataHandler.h"
#include "fileHandler.h"
#include "libwst.h"

typedef struct {
  const char** contentPaths;
  const size_t contentsQuantity;
  Buffer* buffers;
} ReadThreadParams;

typedef struct {
  const char* archivePath;
  Buffer* buffers;
} WriteThreadParams;

#endif
