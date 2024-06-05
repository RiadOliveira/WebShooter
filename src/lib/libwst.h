#ifndef LIB_WEB_SHOOTER_H
#define LIB_WEB_SHOOTER_H

#include <shortcutTypes.h>

void webContentsIntoArchive(
  const char* cwd, const char* outputArchivePath, const char** contentsPaths,
  uint contentsQuantity
);
void unwebArchiveIntoContents(const char* cwd, const char* archivePath);

#endif
