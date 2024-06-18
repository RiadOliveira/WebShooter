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

  ContentData currentData;
  char contentName[CONTENT_NAME_MAX_SIZE];
  currentData.name = contentName;

  for(size_t ind = 0; ind < parsedParams->contentsQuantity; ind++) {
    const char* path = parsedParams->contentPaths[ind];
    fillContentData(&currentData, path);
    redirectContentToHandler(&currentData, buffers, path);
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

void webFolderIntoBuffer(ContentData* data, Buffer* buffers, const char* path) {
  DIR* folder = opendir(path);
  concatPathSeparatorToFolderName(data->name);

  parseBufferForWebbing(data, buffers);
  handleFolderSubContentsReadingIntoBuffer(folder, buffers, path);

  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;

  bool reachedMaxSize = buffers[bufferInd].size == BUFFER_MAX_SIZE;
  if(reachedMaxSize) advanceBufferAndWaitForNext(buffers, &bufferInd);

  Buffer* currentBuffer = &buffers[bufferInd];
  currentBuffer->data[currentBuffer->size++] = PATH_SEPARATOR;

  closedir(folder);
}

void handleFolderSubContentsReadingIntoBuffer(
  DIR* folder, Buffer* buffers, const char* path
) {
  ContentData subContentData;
  const size_t pathLength = strlen(path);

  char currentFullPath[PATH_MAX_SIZE];
  strcpy(currentFullPath, path);

  struct dirent* subContentDirent;
  while((subContentDirent = readdir(folder)) != NULL) {
    if(isEmptySubContent(subContentDirent->d_name)) continue;

    appendPath(currentFullPath, pathLength, subContentDirent->d_name);
    subContentData.name = subContentDirent->d_name;
    subContentData.size = getContentSize(currentFullPath);
    redirectContentToHandler(&subContentData, buffers, currentFullPath);
  }
}

void webFileIntoBuffer(ContentData* data, Buffer* buffers, const char* path) {
  FILE* content = openFileOrExit(path, READ_BINARY_MODE);

  uint bufferInd = parseBufferForWebbing(data, buffers);
  Buffer* currentBuffer = &buffers[bufferInd];

  size_t bytesRead;
  do {
    const size_t currentSize = currentBuffer->size;
    const size_t maxSizeReadable = BUFFER_MAX_SIZE - currentSize;

    byte* currentData = &currentBuffer->data[currentSize];
    bytesRead = fread(currentData, 1, maxSizeReadable, content);
    currentBuffer->size += bytesRead;

    if(maxSizeReadable == bytesRead) {
      advanceBufferAndWaitForNext(buffers, &bufferInd);
      currentBuffer = &buffers[bufferInd];
    }
  } while(bytesRead > 0);

  fclose(content);
}

inline void redirectContentToHandler(
  ContentData* data, Buffer* buffers, const char* path
) {
  bool folder = isFolder(data);
  (folder ? webFolderIntoBuffer : webFileIntoBuffer)(data, buffers, path);
}

uint parseBufferForWebbing(ContentData* data, Buffer* buffers) {
  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;

  const bool isFile = data->size > 0;
  const size_t nameSize = strlen(data->name) + isFile;
  const size_t sizeOfSizeT = isFile ? sizeof(size_t) : 0;
  const size_t sizeToAdd = nameSize + sizeOfSizeT;

  bool exceedsMaxSize = buffers[bufferInd].size + sizeToAdd > BUFFER_MAX_SIZE;
  if(exceedsMaxSize) advanceBufferAndWaitForNext(buffers, &bufferInd);
  Buffer* currentBuffer = &buffers[bufferInd];

  byte* bufferData = &currentBuffer->data[currentBuffer->size];
  currentBuffer->size += sizeToAdd;

  memcpy(bufferData, data->name, nameSize);
  bufferData += nameSize;
  memcpy(bufferData, &data->size, sizeOfSizeT);
  bufferData += sizeOfSizeT;

  return bufferInd;
}
