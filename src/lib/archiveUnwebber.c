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

  pthread_join(readThread, NULL);  // This will be removed after writing impl.
  pthread_join(writingThread, NULL);
  finalizeBuffers(buffers, BUFFERS_QUANTITY);
}

void* handleArchiveReading(void* params) {
  ReadThreadParams* parsedParams = (ReadThreadParams*)params;
  FILE* archive = openFileOrExit(parsedParams->archivePath, READ_BINARY_MODE);

  uint bufferInd = 0;
  Buffer* currentBuffer;
  do {
    currentBuffer = &parsedParams->buffers[bufferInd];
    byte* currentData = currentBuffer->data;
    currentBuffer->size = fread(currentData, 1, BUFFER_MAX_SIZE, archive);

    advanceBufferAndWait(parsedParams->buffers, &bufferInd);
  } while(currentBuffer->size > 0);

  fclose(archive);
}

void* handleContentsWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
}
