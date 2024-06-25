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
  Buffer* buffers = parsedParams->buffers;

  ContentData data;
  char contentName[CONTENT_NAME_MAX_SIZE], fullPath[PATH_MAX_SIZE];
  data.name = contentName;

  for(size_t ind = 0; ind < parsedParams->contentsQuantity; ind++) {
    const char* path = parsedParams->contentPaths[ind];
    fillContentData(&data, path);

    if(isFile(&data)) webFileIntoBuffer(&data, buffers, path);
    else {
      strcpy(fullPath, path);
      webFolderIntoBuffer(&data, buffers, fullPath, strlen(path));
    }
  }

  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;
  advanceBufferAndWaitForNext(buffers, &bufferInd);
  buffers[bufferInd].status = EMPTY;
}

void* handleArchiveWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
  FILE* archive = openFileOrExit(parsedParams->archivePath, WRITE_BINARY_MODE);

  uint bufferInd = 0;
  Buffer* currentBuffer = &parsedParams->buffers[bufferInd];

  do {
    waitBufferReachStatus(currentBuffer, READABLE);
    fwrite(currentBuffer->data, 1, currentBuffer->size, archive);

    currentBuffer->size = currentBuffer->status = UNINITIALIZED;
    pthread_cond_signal(&currentBuffer->cond);

    if(++bufferInd == BUFFERS_QUANTITY) bufferInd = 0;
    currentBuffer = &parsedParams->buffers[bufferInd];
  } while(currentBuffer->status != EMPTY);

  fclose(archive);
}

void webFolderIntoBuffer(
  ContentData* data, Buffer* buffers, char* fullPath, size_t pathLength
) {
  DIR* folder = openFolderOrExit(fullPath);
  concatPathSeparatorToFolderName(data->name);

  parseBufferForWebbing(data, buffers);
  handleFolderSubContentsReadingIntoBuffer(
    folder, buffers, fullPath, pathLength
  );

  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;

  bool reachedMaxSize = buffers[bufferInd].size == BUFFER_MAX_SIZE;
  if(reachedMaxSize) advanceBufferAndWaitForNext(buffers, &bufferInd);

  Buffer* currentBuffer = &buffers[bufferInd];
  currentBuffer->data[currentBuffer->size++] = PATH_SEPARATOR;

  closedir(folder);
}

void handleFolderSubContentsReadingIntoBuffer(
  DIR* folder, Buffer* buffers, char* fullPath, size_t pathLength
) {
  ContentData subData;
  struct dirent* subContentDirent;

  while((subContentDirent = readdir(folder)) != NULL) {
    if(isEmptySubContent(subContentDirent->d_name)) continue;
    appendPath(fullPath, pathLength, subContentDirent->d_name);

    subData.name = subContentDirent->d_name;
    setContentMetadata(&subData.metadata, fullPath);

    if(isFile(&subData)) webFileIntoBuffer(&subData, buffers, fullPath);
    else {
      const size_t fullLength = pathLength + strlen(&fullPath[pathLength]);
      webFolderIntoBuffer(&subData, buffers, fullPath, fullLength);
    }
  }
}

void webFileIntoBuffer(ContentData* data, Buffer* buffers, const char* path) {
  FILE* content = openFileOrExit(path, READ_BINARY_MODE);

  uint bufferInd = parseBufferForWebbing(data, buffers);
  Buffer* currentBuffer = &buffers[bufferInd];

  bool hasMoreBytesToRead;
  do {
    const size_t currentSize = currentBuffer->size;
    const size_t maxSizeReadable = BUFFER_MAX_SIZE - currentSize;

    byte* currentData = &currentBuffer->data[currentSize];
    const size_t bytesRead = fread(currentData, 1, maxSizeReadable, content);
    currentBuffer->size += bytesRead;

    if(hasMoreBytesToRead = bytesRead == maxSizeReadable) {
      advanceBufferAndWaitForNext(buffers, &bufferInd);
      currentBuffer = &buffers[bufferInd];
    }
  } while(hasMoreBytesToRead);

  fclose(content);
}

inline void redirectContentToHandler(
  ContentData* data, Buffer* buffers, const char* path, char* fullPath
) {
  if(!isFolder(data)) webFileIntoBuffer(data, buffers, path);
  else {
    strcpy(fullPath, path);
    webFolderIntoBuffer(data, buffers, fullPath, strlen(path));
  }
}

uint parseBufferForWebbing(ContentData* data, Buffer* buffers) {
  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;

  const bool file = isFile(data);
  const size_t nameSize = strlen(data->name) + file;
  const size_t metadataSize = sizeof(Metadata) - (file ? 0 : sizeof(size_t));
  const size_t sizeToAdd = nameSize + metadataSize;

  bool reachesMaxSize = buffers[bufferInd].size + sizeToAdd >= BUFFER_MAX_SIZE;
  if(reachesMaxSize) advanceBufferAndWaitForNext(buffers, &bufferInd);

  Buffer* currentBuffer = &buffers[bufferInd];
  byte* bufferData = &currentBuffer->data[currentBuffer->size];

  memcpy(bufferData, data->name, nameSize);
  memcpy(&bufferData[nameSize], &data->metadata, metadataSize);
  currentBuffer->size += sizeToAdd;

  return bufferInd;
}
