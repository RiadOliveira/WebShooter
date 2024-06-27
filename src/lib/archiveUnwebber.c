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
  UnwebbingData data = {parsedParams->buffers, 0};
  Metadata* metadata = &data.contentData.metadata;

  Buffer* buffers = data.buffers;
  uint* bufferInd = &data.bufferInd;
  waitForBufferStatusMismatch(&buffers[*bufferInd], UNSET);
  do {
    getContentDataFromBuffers(&data);

    if(isFolder(metadata)) unwebFolderFromBuffers(&data);
    else unwebFileFromBuffers(&data);
  } while(buffers[*bufferInd].status != FINISHED);
}

inline void getContentDataFromBuffers(UnwebbingData* data) {
  getContentNameFromBuffers(data);
  getContentMetadataFromBuffers(data);
}

inline void getContentNameFromBuffers(UnwebbingData* data) {
  char* name = data->contentData.name;
  uint nameInd = 0;

  do consumeBuffersBytes(&name[nameInd], data->buffers, &data->bufferInd, 1);
  while(name[nameInd++] != NULL_TERMINATOR);
}

inline void getContentMetadataFromBuffers(UnwebbingData* data) {
  Buffer* buffers = data->buffers;
  uint* bufferInd = &data->bufferInd;

  Metadata* metadata = &data->contentData.metadata;
  byte* metadataBytes = (byte*)metadata;

  consumeBuffersBytes(metadataBytes, buffers, bufferInd, METADATA_MODE_SIZE);
  metadataBytes += METADATA_MODE_SIZE;

  const size_t sizeLeft = getMetadataStructSize(metadata) - METADATA_MODE_SIZE;
  consumeBuffersBytes(metadataBytes, buffers, bufferInd, sizeLeft);
}

void unwebFolderFromBuffers(UnwebbingData* data) {}

void unwebFileFromBuffers(UnwebbingData* data) {}
