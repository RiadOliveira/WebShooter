#ifndef LIB_WEB_SHOOTER_H
#define LIB_WEB_SHOOTER_H

#include <stdlib.h>

typedef struct {
  const char* archivePath;
  const char** contentPaths;
  size_t contentsQuantity;
} WstParams;

void webContentsIntoArchive(WstParams* params);
void unwebArchiveIntoContents(WstParams* params);

#endif
