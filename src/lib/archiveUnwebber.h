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

typedef struct {
  Buffer* buffers;
  uint bufferInd;
  ContentData contentData;
} UnwebbingData;

void* handleArchiveReading(void*);
void* handleContentsWriting(void*);

void getContentDataFromBuffers(UnwebbingData*);
void getContentNameFromBuffers(UnwebbingData*);
void getContentMetadataFromBuffers(UnwebbingData*);

void unwebFolderFromBuffers(UnwebbingData*);
void unwebFileFromBuffers(UnwebbingData*);

#endif
