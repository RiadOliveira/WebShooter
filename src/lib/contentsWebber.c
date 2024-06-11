#include "contentsWebber.h"

void* handleContentsReading(void*);
void webFolderIntoBuffer();
void webFileIntoBuffer(ContentData*, Buffer*, const char*);

void parseBufferForWebbing(ContentData*, Buffer*, uint*);
void advanceBufferAndWait(Buffer*, uint*);

void* handleArchiveWriting(void*);

void webContentsIntoArchive(WstParams* params, const char* archivePath) {
  char currentFullPath[PATH_MAX_SIZE];
  strcpy(currentFullPath, params->cwd);

  const size_t cwdLength = strlen(params->cwd);
  appendPath(currentFullPath, cwdLength, archivePath);
  FILE* archive = openFileOrExit(currentFullPath, "wb");

  Buffer buffers[BUFFERS_QUANTITY];
  pthread_t readThread, writingThread;

  ReadThreadParams readParams = {params, buffers, currentFullPath, cwdLength};
  pthread_create(&readThread, NULL, handleContentsReading, &readParams);

  WriteThreadParams writingParams = {archive, buffers};
  pthread_create(&writingThread, NULL, handleArchiveWriting, &writingParams);

  pthread_join(readThread, NULL);
  pthread_join(writingThread, NULL);

  fclose(archive);
}

void* handleContentsReading(void* params) {
  ReadThreadParams* parsedParams = (ReadThreadParams*)params;

  Buffer* buffers = parsedParams->buffers;
  for(size_t ind = 0; ind < BUFFERS_QUANTITY; ind++) {
    buffers[ind].size = buffers[ind].status = UNINITIALIZED;
  }

  ContentData currentData;
  char* currentFullPath = parsedParams->currentFullPath;
  const WstParams* wstParams = parsedParams->wstParams;

  for(size_t ind = 0; ind < wstParams->contentsOrArchivesQuantity; ind++) {
    const char* currentPath = wstParams->contentOrArchivePaths[ind];

    getContentName(currentData.name, currentPath);
    appendPath(currentFullPath, parsedParams->cwdLength, currentPath);
    currentData.size = getContentSize(currentFullPath);

    webFileIntoBuffer(&currentData, buffers, currentFullPath);
  }
}

void webFolderIntoBuffer();

void webFileIntoBuffer(ContentData* data, Buffer* buffers, const char* path) {
  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;
  parseBufferForWebbing(data, buffers, &bufferInd);

  Buffer selectedBuffer = buffers[bufferInd];
  size_t currentSize = selectedBuffer.size;
  byte* currentData = &selectedBuffer.data[currentSize];

  FILE* content = openFileOrExit(path, "rb");
  size_t bytesRead, expectedToRead;
  do {
    expectedToRead = BUFFER_MAX_SIZE - currentSize;
    bytesRead = fread(currentData, 1, expectedToRead, content);

    if(expectedToRead == bytesRead) {
      advanceBufferAndWait(buffers, &bufferInd);
      currentData = buffers[bufferInd].data;
    }
  } while(bytesRead > 0);

  fclose(content);
}

void parseBufferForWebbing(
  ContentData* data, Buffer* buffers, uint* bufferInd
) {
  const size_t nameSize = strlen(data->name) + 1;
  const size_t sizeOfSizeT = sizeof(size_t);
  const size_t sizeToAdd = nameSize + sizeOfSizeT;

  Buffer currentBuffer = buffers[*bufferInd];
  bool exceedsMaxSize = currentBuffer.size + sizeToAdd > BUFFER_MAX_SIZE;
  if(exceedsMaxSize) advanceBufferAndWait(buffers, bufferInd);

  byte* bufferData = currentBuffer.data;
  currentBuffer.size += sizeToAdd;

  memcpy(bufferData, data->name, nameSize);
  bufferData += nameSize;
  memcpy(bufferData, &data->size, sizeOfSizeT);
  bufferData += sizeOfSizeT;
}

inline void advanceBufferAndWait(Buffer* buffers, uint* bufferInd) {
  buffers[*bufferInd].status = READABLE;
  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;

  while(buffers[*bufferInd].status != UNINITIALIZED);
}

void* handleArchiveWriting(void* params) {
  WriteThreadParams* parsedParams = (WriteThreadParams*)params;
}
