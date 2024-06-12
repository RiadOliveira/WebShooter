#ifndef CONTENTS_WEBBER_H
#define CONTENTS_WEBBER_H

#include <dirent.h>

#include "dataHandler.h"
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

void webFolderIntoBuffer(ContentData*, Buffer*, const char*, bool);
void handleFolderSubContentsReadingIntoBuffer(DIR*, Buffer*, const char*);
void finalizeFolderWebbingIntoBuffer(Buffer*, bool);

void webFileIntoBuffer(ContentData*, Buffer*, const char*, bool);
void handleFileReadingIntoBuffer(FILE*, Buffer*, uint*);

void redirectContentToHandler(ContentData*, Buffer*, const char*, bool);
uint parseBufferForWebbing(ContentData*, Buffer*);
void advanceBufferAndWait(Buffer*, uint*);

#endif
