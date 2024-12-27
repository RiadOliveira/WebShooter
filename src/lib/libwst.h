#ifndef LIB_WST
#define LIB_WST

#include <stdlib.h>

typedef struct {
  const char* archivePath;
  const char** contentPaths;
  size_t contentsQuantity;
} WstParams;

void webContentsIntoArchive(WstParams* params);
void unwebArchiveIntoContents(WstParams* params);

#endif
