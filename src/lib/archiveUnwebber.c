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
  Buffer* buffers = parsedParams->buffers;
  FILE* archive = openFileOrExit(parsedParams->archivePath, READ_BINARY_MODE);

  uint bufferInd = 0;
  bool hasMoreBytesToRead;
  do {
    Buffer* currentBuffer = &buffers[bufferInd];

    byte* currentData = currentBuffer->data;
    currentBuffer->size = fread(currentData, 1, BUFFER_MAX_SIZE, archive);
    currentBuffer->consumedSize = 0;

    if(hasMoreBytesToRead = (currentBuffer->size > 0)) {
      setBufferStatusAndWaitForNext(READABLE, buffers, &bufferInd);
    }
  } while(hasMoreBytesToRead);

  finishBuffersReading(buffers, &bufferInd);
  fclose(archive);
}

void* handleContentsWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
  Buffer* buffers = parsedParams->buffers;

  ContentData data;
  uint bufferInd = 0;
  waitForBufferStatusMismatch(&buffers[bufferInd], UNSET);
  do {
    getContentDataFromBuffers(&data, buffers, &bufferInd);
  } while(buffers[bufferInd].status != FINISHED);
}

inline void getContentDataFromBuffers(
  ContentData* data, Buffer* buffers, uint* bufferInd
) {
  getContentNameFromBuffers(data->name, buffers, bufferInd);
  getContentMetadataFromBuffers(&data->metadata, buffers, bufferInd);
}

inline void getContentNameFromBuffers(
  char* name, Buffer* buffers, uint* bufferInd
) {
  uint nameInd = 0;
  do consumeBuffersBytes(&name[nameInd], buffers, bufferInd, 1);
  while(name[nameInd++] != NULL_TERMINATOR);
}

inline void getContentMetadataFromBuffers(
  Metadata* metadata, Buffer* buffers, uint* bufferInd
) {
  byte* metadataBytes = (byte*)metadata;
  consumeBuffersBytes(metadataBytes, buffers, bufferInd, METADATA_MODE_SIZE);
  metadataBytes += METADATA_MODE_SIZE;

  const size_t sizeLeft = getMetadataStructSize(metadata) - METADATA_MODE_SIZE;
  consumeBuffersBytes(metadataBytes, buffers, bufferInd, sizeLeft);
}
