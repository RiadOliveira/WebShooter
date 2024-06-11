#include "contentsWebber.h"

void* handleContentsReading(void*);
void webFolderIntoBuffer(ContentData*, Buffer*, const char*, bool);
void webFileIntoBuffer(ContentData*, Buffer*, const char*, bool);

void parseBufferForWebbing(ContentData*, Buffer*, uint*);
void advanceBufferAndWait(Buffer*, uint*);

void* handleArchiveWriting(void*);

void webContentsIntoArchive(WstParams* params, const char* archivePath) {
  Buffer buffers[BUFFERS_QUANTITY];
  initializeBuffers(buffers, BUFFERS_QUANTITY);

  pthread_t readThread;
  ReadThreadParams readParams = {
    params->contentOrArchivePaths, params->contentsOrArchivesQuantity, buffers
  };
  pthread_create(&readThread, NULL, handleContentsReading, &readParams);

  pthread_t writingThread;
  WriteThreadParams writingParams = {archivePath, buffers};
  pthread_create(&writingThread, NULL, handleArchiveWriting, &writingParams);

  pthread_join(writingThread, NULL);
  finalizeBuffers(buffers, BUFFERS_QUANTITY);
}

inline void* handleContentsReading(void* params) {
  ReadThreadParams* parsedParams = (ReadThreadParams*)params;
  Buffer* buffers = parsedParams->buffers;

  ContentData currentData;
  for(size_t ind = 0; ind < parsedParams->contentsQuantity; ind++) {
    const char* path = parsedParams->contentPaths[ind];
    fillContentData(&currentData, path);

    bool isFolder = currentData.size == 0;
    bool lastContent = ind == parsedParams->contentsQuantity - 1;

    if(isFolder) webFolderIntoBuffer(&currentData, buffers, path, lastContent);
    else webFileIntoBuffer(&currentData, buffers, path, lastContent);
  }
}

void webFolderIntoBuffer(
  ContentData* data, Buffer* buffers, const char* path, bool lastContent
) {}

void webFileIntoBuffer(
  ContentData* data, Buffer* buffers, const char* path, bool lastContent
) {
  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;
  parseBufferForWebbing(data, buffers, &bufferInd);

  FILE* content = openFileOrExit(path, "rb");
  Buffer* currentBuffer = &buffers[bufferInd];

  size_t bytesRead;
  do {
    const size_t currentSize = currentBuffer->size;
    const size_t maxSizeReadable = BUFFER_MAX_SIZE - currentSize;

    byte* currentData = &currentBuffer->data[currentSize];
    bytesRead = fread(currentData, 1, maxSizeReadable, content);
    currentBuffer->size += bytesRead;

    if(maxSizeReadable == bytesRead) {
      advanceBufferAndWait(buffers, &bufferInd);
      currentBuffer = &buffers[bufferInd];
    }
  } while(bytesRead > 0);

  if(lastContent) {
    advanceBufferAndWait(buffers, &bufferInd);
    buffers[bufferInd].status = EMPTY;
  }

  fclose(content);
}

void parseBufferForWebbing(
  ContentData* data, Buffer* buffers, uint* bufferInd
) {
  const size_t nameSize = strlen(data->name) + 1;
  const size_t sizeOfSizeT = sizeof(size_t);
  const size_t sizeToAdd = nameSize + sizeOfSizeT;

  Buffer* currentBuffer = &buffers[*bufferInd];
  bool exceedsMaxSize = currentBuffer->size + sizeToAdd > BUFFER_MAX_SIZE;
  if(exceedsMaxSize) advanceBufferAndWait(buffers, bufferInd);

  byte* bufferData = currentBuffer->data;
  currentBuffer->size += sizeToAdd;

  memcpy(bufferData, data->name, nameSize);
  bufferData += nameSize;
  memcpy(bufferData, &data->size, sizeOfSizeT);
  bufferData += sizeOfSizeT;
}

inline void advanceBufferAndWait(Buffer* buffers, uint* bufferInd) {
  Buffer* selectedBuffer = &buffers[*bufferInd];
  selectedBuffer->status = READABLE;
  pthread_cond_signal(&selectedBuffer->cond);

  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;
  selectedBuffer = &buffers[*bufferInd];
  pthread_mutex_t* mutex = &selectedBuffer->mutex;

  pthread_mutex_lock(mutex);
  while(selectedBuffer->status != UNINITIALIZED) {
    pthread_cond_wait(&selectedBuffer->cond, mutex);
  }
  pthread_mutex_unlock(mutex);
}

void* handleArchiveWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
  FILE* archive = openFileOrExit(parsedParams->archivePath, "wb");

  uint bufferInd = 0;
  Buffer* selectedBuffer = &parsedParams->buffers[bufferInd];
  pthread_mutex_t* mutex = &selectedBuffer->mutex;

  do {
    pthread_mutex_lock(mutex);
    while(selectedBuffer->status != READABLE) {
      pthread_cond_wait(&selectedBuffer->cond, mutex);
    }
    pthread_mutex_unlock(mutex);

    fwrite(selectedBuffer->data, 1, selectedBuffer->size, archive);
    selectedBuffer->size = selectedBuffer->status = UNINITIALIZED;
    pthread_cond_signal(&selectedBuffer->cond);

    if(++bufferInd == BUFFERS_QUANTITY) bufferInd = 0;
    selectedBuffer = &parsedParams->buffers[bufferInd];
    mutex = &selectedBuffer->mutex;
  } while(selectedBuffer->status != EMPTY);

  fclose(archive);
}
