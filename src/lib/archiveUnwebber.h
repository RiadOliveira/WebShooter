#ifndef ARCHIVE_UNWEBBER_H
#define ARCHIVE_UNWEBBER_H

#include "bufferHandler.h"
#include "contentHandler.h"
#include "libwst.h"

typedef struct {
  const char* archivePath;
  Buffer* buffers;
} ReadThreadParams;

typedef struct {
  const char* contentsPath;
  Buffer* buffers;
} WriteThreadParams;

void* handleArchiveReading(void*);
void* handleContentsWriting(void*);

#endif
