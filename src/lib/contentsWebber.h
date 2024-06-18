#ifndef CONTENTS_WEBBER_H
#define CONTENTS_WEBBER_H

#include <dirent.h>

#include "bufferHandler.h"
#include "contentHandler.h"
#include "fileHandler.h"
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

void webFolderIntoBuffer(ContentData*, Buffer*, const char*);
void handleFolderSubContentsReadingIntoBuffer(DIR*, Buffer*, const char*);

void webFileIntoBuffer(ContentData*, Buffer*, const char*);

void redirectContentToHandler(ContentData*, Buffer*, const char*);
uint parseBufferForWebbing(ContentData*, Buffer*);

#endif
