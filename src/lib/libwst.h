#ifndef LIB_WEB_SHOOTER_H
#define LIB_WEB_SHOOTER_H

#include "libTypes.h"
#include "typeChecker.h"

void webContentsIntoArchive(WstParams* params, const char* archivePath);

void unwebArchiveIntoContents(WstParams* params);

#endif
