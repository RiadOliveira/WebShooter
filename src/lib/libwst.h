#ifndef LIB_WST_H
#define LIB_WST_H

#include <stdlib.h>

typedef struct {
  const char* archivePath;
  const char** contentPaths;
  size_t contentsQuantity;
} WstParams;

void webContentsIntoArchive(WstParams* params);
void unwebArchiveIntoContents(WstParams* params);

#endif
