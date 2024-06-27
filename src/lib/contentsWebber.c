#include "contentsWebber.h"

void webContentsIntoArchive(WstParams* params) {
  Buffer buffers[BUFFERS_QUANTITY];
  initializeBuffers(buffers, BUFFERS_QUANTITY);

  pthread_t readThread;
  ReadThreadParams readParams = {
    params->contentPaths, params->contentsQuantity, buffers
  };
  pthread_create(&readThread, NULL, handleContentsReading, &readParams);

  pthread_t writingThread;
  WriteThreadParams writingParams = {params->archivePath, buffers};
  pthread_create(&writingThread, NULL, handleArchiveWriting, &writingParams);

  pthread_join(writingThread, NULL);
  finalizeBuffers(buffers, BUFFERS_QUANTITY);
}

void* handleContentsReading(void* params) {
  ReadThreadParams* parsedParams = (ReadThreadParams*)params;
  WebbingData data = {parsedParams->buffers, 0};

  for(size_t ind = 0; ind < parsedParams->contentsQuantity; ind++) {
    strcpy(data.fullPath, parsedParams->contentPaths[ind]);
    fillContentData(&data.contentData, data.fullPath);

    if(isFile(&data.contentData.metadata)) webFileIntoBuffers(&data);
    else webFolderIntoBuffers(&data, strlen(data.fullPath));
  }

  finishBuffersReading(data.buffers, &data.bufferInd);
}

void* handleArchiveWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
  Buffer* buffers = parsedParams->buffers;
  FILE* archive = openFileOrExit(parsedParams->archivePath, WRITE_BINARY_MODE);

  uint bufferInd = 0;
  Buffer* currentBuffer = &buffers[bufferInd];
  waitForBufferStatusMismatch(currentBuffer, UNSET);

  do {
    fwrite(currentBuffer->data, 1, currentBuffer->size, archive);
    currentBuffer->size = 0;

    setBufferStatusAndWaitForNext(UNSET, buffers, &bufferInd);
    currentBuffer = &buffers[bufferInd];
  } while(currentBuffer->status != FINISHED);

  fclose(archive);
}

void webFolderIntoBuffers(WebbingData* data, size_t pathLength) {
  DIR* folder = openFolderOrExit(data->fullPath);
  parseBuffersForWebbing(data);
  webFolderSubContentsIntoBuffers(folder, data, pathLength);

  Buffer* buffers = data->buffers;
  uint* bufferInd = &data->bufferInd;

  getIndOfFirstBufferWithStatus(buffers, bufferInd, UNSET);
  const bool reachedMaxSize = buffers[*bufferInd].size == BUFFER_MAX_SIZE;
  if(reachedMaxSize) {
    setBufferStatusAndWaitForNext(READABLE, buffers, bufferInd);
  }

  Buffer* currentBuffer = &buffers[*bufferInd];
  currentBuffer->data[currentBuffer->size++] = PATH_SEPARATOR;

  closedir(folder);
}

void webFolderSubContentsIntoBuffers(
  DIR* folder, WebbingData* data, size_t pathLength
) {
  char* fullPath = data->fullPath;
  ContentData* contentData = &data->contentData;
  struct dirent* subContentDirent;

  while((subContentDirent = readdir(folder)) != NULL) {
    if(isEmptySubContent(subContentDirent->d_name)) continue;

    appendPath(fullPath, pathLength, subContentDirent->d_name);
    strcpy(contentData->name, subContentDirent->d_name);
    setContentMetadata(&contentData->metadata, fullPath);

    if(isFile(&contentData->metadata)) webFileIntoBuffers(data);
    else {
      const size_t fullLength = pathLength + strlen(&fullPath[pathLength]);
      webFolderIntoBuffers(data, fullLength);
    }
  }
}

void webFileIntoBuffers(WebbingData* data) {
  FILE* content = openFileOrExit(data->fullPath, READ_BINARY_MODE);
  parseBuffersForWebbing(data);

  Buffer* buffers = data->buffers;
  uint* bufferInd = &data->bufferInd;
  bool hasMoreBytesToRead;
  do {
    Buffer* currentBuffer = &buffers[*bufferInd];

    const size_t currentSize = currentBuffer->size;
    const size_t maxSizeReadable = BUFFER_MAX_SIZE - currentSize;

    byte* currentData = &currentBuffer->data[currentSize];
    const size_t bytesRead = fread(currentData, 1, maxSizeReadable, content);
    currentBuffer->size += bytesRead;

    if(hasMoreBytesToRead = (bytesRead == maxSizeReadable)) {
      setBufferStatusAndWaitForNext(READABLE, buffers, bufferInd);
    }
  } while(hasMoreBytesToRead);

  fclose(content);
}

void parseBuffersForWebbing(WebbingData* data) {
  Buffer* buffers = data->buffers;
  uint* bufferInd = &data->bufferInd;
  getIndOfFirstBufferWithStatus(buffers, bufferInd, UNSET);

  char* name = data->contentData.name;
  Metadata* metadata = &data->contentData.metadata;
  const size_t nameSize = strlen(name) + 1;
  const size_t metadataSize = getMetadataStructSize(metadata);
  const size_t sizeToAdd = nameSize + metadataSize;

  const size_t currentSize = buffers[*bufferInd].size;
  const bool reachesMaxSize = currentSize + sizeToAdd >= BUFFER_MAX_SIZE;
  if(reachesMaxSize) {
    setBufferStatusAndWaitForNext(READABLE, buffers, bufferInd);
  }

  Buffer* currentBuffer = &buffers[*bufferInd];
  byte* bufferData = &currentBuffer->data[currentBuffer->size];

  memcpy(bufferData, name, nameSize);
  memcpy(&bufferData[nameSize], metadata, metadataSize);
  currentBuffer->size += sizeToAdd;
}
