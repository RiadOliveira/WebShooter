#ifndef ARCHIVE_UNWEBBER
#define ARCHIVE_UNWEBBER

#include "libwst.h"
#include "operationTypes.h"

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

void getContentDataFromBuffers(WebbingOperationData*);
void getContentNameFromBuffers(WebbingOperationData*);
void getContentMetadataFromBuffers(WebbingOperationData*);

void unwebFolderFromBuffers(WebbingOperationData*);
void unwebFileFromBuffers(WebbingOperationData*);

void unwebContent(WebbingOperationData*);

#endif
