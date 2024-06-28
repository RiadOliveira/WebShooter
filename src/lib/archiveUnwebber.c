#include "archiveUnwebber.h"

void unwebArchiveIntoContents(WstParams* params) {
  Buffer buffers[BUFFERS_QUANTITY];
  initializeBuffers(buffers, BUFFERS_QUANTITY);

  pthread_t readThread;
  ReadThreadParams readParams = {params->archivePath, buffers};
  pthread_create(&readThread, NULL, handleArchiveReading, &readParams);

  pthread_t writingThread;
  WriteThreadParams writingParams = {
    params->contentPaths == NULL ? CWD : *params->contentPaths, buffers
  };
  pthread_create(&writingThread, NULL, handleContentsWriting, &writingParams);

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
      setBufferStatusAndWaitForNext(buffers, &bufferInd, CONSUMABLE);
    }
  } while(hasMoreBytesToRead);

  finishBuffersReading(buffers, &bufferInd);
  fclose(archive);
}

void* handleContentsWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
  const size_t basePathLength = strlen(parsedParams->contentsPath);

  WebbingOperationData data = {parsedParams->buffers, 0};
  strcpy(data.fullPath, parsedParams->contentsPath);

  Buffer* buffers = data.buffers;
  uint* bufferInd = &data.bufferInd;
  waitForBufferStatusMismatch(&buffers[*bufferInd], UNSET);

  do {
    getContentDataFromBuffers(&data);
    appendPath(data.fullPath, basePathLength, data.contentData.name);
    unwebContent(&data);
  } while(buffers[*bufferInd].status != FINISHED);
}

inline void getContentDataFromBuffers(WebbingOperationData* data) {
  getContentNameFromBuffers(data);
  getContentMetadataFromBuffers(data);
}

inline void getContentNameFromBuffers(WebbingOperationData* data) {
  char* name = data->contentData.name;
  uint nameInd = 0;

  do consumeBuffersBytes(&name[nameInd], data->buffers, &data->bufferInd, 1);
  while(name[nameInd++] != NULL_TERMINATOR);
}

inline void getContentMetadataFromBuffers(WebbingOperationData* data) {
  Buffer* buffers = data->buffers;
  uint* bufferInd = &data->bufferInd;

  Metadata* metadata = &data->contentData.metadata;
  byte* metadataBytes = (byte*)metadata;

  consumeBuffersBytes(metadataBytes, buffers, bufferInd, METADATA_MODE_SIZE);
  metadataBytes += METADATA_MODE_SIZE;

  const size_t sizeLeft = getMetadataStructSize(metadata) - METADATA_MODE_SIZE;
  consumeBuffersBytes(metadataBytes, buffers, bufferInd, sizeLeft);
}

void unwebFolderFromBuffers(WebbingOperationData* data) {
  char* fullPath = data->fullPath;
  const size_t pathLength = strlen(fullPath);
  createFolder(fullPath);

  Buffer* currentBuffer = &data->buffers[data->bufferInd];
  while(getBufferCurrentByte(currentBuffer) != FOLDER_TERMINATOR) {
    getContentDataFromBuffers(data);
    appendPath(fullPath, pathLength, data->contentData.name);
    unwebContent(data);

    currentBuffer = &data->buffers[data->bufferInd];
  }

  const size_t consumedSizeAfterTerminator = ++currentBuffer->consumedSize;
  if(consumedSizeAfterTerminator == currentBuffer->size) {
    setBufferStatusAndWaitForNext(data->buffers, &data->bufferInd, UNSET);
  }

  fullPath[pathLength] = NULL_TERMINATOR;
}

void unwebFileFromBuffers(WebbingOperationData* data) {
  FILE* file = openFileOrExit(data->fullPath, WRITE_BINARY_MODE);

  const size_t fileSize = data->contentData.metadata.size;
  size_t bytesWritten = 0;
  do {
    Buffer* currentBuffer = &data->buffers[data->bufferInd];
    size_t* consumedSize = &currentBuffer->consumedSize;

    const size_t remainingBufferSize = currentBuffer->size - *consumedSize;
    const size_t remainingFileSize = fileSize - bytesWritten;
    const size_t sizeToConsume = min(remainingBufferSize, remainingFileSize);

    byte* currentData = &currentBuffer->data[*consumedSize];
    *consumedSize += fwrite(currentData, 1, sizeToConsume, file);
    bytesWritten += sizeToConsume;

    if(sizeToConsume == remainingBufferSize) {
      setBufferStatusAndWaitForNext(data->buffers, &data->bufferInd, UNSET);
    }
  } while(bytesWritten < fileSize);

  fclose(file);
}

inline void unwebContent(WebbingOperationData* data) {
  Metadata metadata = data->contentData.metadata;

  const bool folder = isFolder(&metadata);
  (folder ? unwebFolderFromBuffers : unwebFileFromBuffers)(data);

  setFileOrFolderMetadata(data->fullPath, &metadata);
}
