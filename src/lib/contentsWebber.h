#ifndef CONTENTS_WEBBER_H
#define CONTENTS_WEBBER_H

#include "bufferHandler.h"
#include "contentHandler.h"
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

void* handleContentsReading(void*);
void* handleArchiveWriting(void*);

void webFolderIntoBuffer(ContentData*, Buffer*, char*, size_t);
void handleFolderSubContentsReadingIntoBuffer(DIR*, Buffer*, char*, size_t);
void webFileIntoBuffer(ContentData*, Buffer*, const char*);
uint parseBufferForWebbing(ContentData*, Buffer*);

void finishContentsReading(Buffer* buffers);

#endif
