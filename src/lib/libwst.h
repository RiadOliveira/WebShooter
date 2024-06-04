#ifndef LIB_WEB_SHOOTER_H
#define LIB_WEB_SHOOTER_H

#include <shortcutTypes.h>

void webContentsIntoArchive(
  char* outputArchivePath, char** contents, uint contentsQuantity
);
void unwebArchiveIntoContents(char* archivePath);

#endif
