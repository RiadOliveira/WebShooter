#ifndef CONTENTS_WEBBER
#define CONTENTS_WEBBER

#include "libwst.h"
#include "operationTypes.h"

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

void webFolderIntoBuffers(WebbingOperationData*);
void webFolderSubContentsIntoBuffers(DIR*, WebbingOperationData*);
void webFileIntoBuffers(WebbingOperationData*);

void webContent(WebbingOperationData*);
void parseBuffersForWebbing(WebbingOperationData*);

#endif
