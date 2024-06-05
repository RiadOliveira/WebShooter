#ifndef LIB_WEB_SHOOTER_H
#define LIB_WEB_SHOOTER_H

#include "libTypes.h"

void webContentsIntoArchive(
  WstOperationParams* params, const char* outputArchivePath
);

void unwebArchiveIntoContents(WstOperationParams* params);

#endif
