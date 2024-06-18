#include "archiveUnwebber.h"

void unwebArchiveIntoContents(WstParams* params) {
  Buffer buffers[BUFFERS_QUANTITY];
  initializeBuffers(buffers, BUFFERS_QUANTITY);

  pthread_t readThread;
  ReadThreadParams readParams = {params->archivePath, buffers};
  pthread_create(&readThread, NULL, handleArchiveReading, &readParams);

  pthread_t writingThread;
  WriteThreadParams writingParams = {*params->contentPaths, buffers};
  pthread_create(&writingThread, NULL, handleContentsWriting, &writingParams);

  pthread_join(writingThread, NULL);
  finalizeBuffers(buffers, BUFFERS_QUANTITY);
}

void* handleArchiveReading(void* params) {}

void* handleContentsWriting(void* params) {}