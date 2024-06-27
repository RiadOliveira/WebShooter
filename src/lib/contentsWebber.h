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

typedef struct {
  Buffer* buffers;
  uint bufferInd;
  ContentData contentData;
  char fullPath[PATH_MAX_SIZE];
} WebbingData;

void* handleContentsReading(void*);
void* handleArchiveWriting(void*);

void webFolderIntoBuffers(WebbingData*, size_t);
void webFolderSubContentsIntoBuffers(DIR*, WebbingData*, size_t);
void webFileIntoBuffers(WebbingData*);
void parseBuffersForWebbing(WebbingData*);

#endif
