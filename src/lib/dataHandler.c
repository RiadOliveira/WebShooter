#include "dataHandler.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

void getContentName(char*, const char*);

inline void initializeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    currentBuffer->size = currentBuffer->status = UNINITIALIZED;
    pthread_mutex_init(&currentBuffer->mutex, NULL);
    pthread_cond_init(&currentBuffer->cond, NULL);
  }
}

inline void finalizeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    pthread_mutex_destroy(&currentBuffer->mutex);
    pthread_cond_destroy(&currentBuffer->cond);
  }
}

inline void fillContentData(ContentData* data, const char* path) {
  struct stat pathStat;
  if(stat(path, &pathStat) != 0) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, path);
  }

  const uint stMode = pathStat.st_mode;
  const bool isFile = S_ISREG(stMode);
  const bool isFolder = S_ISDIR(stMode);
  if(!isFile && !isFolder) printErrorAndExit(LOCATING_ERROR_MESSAGE, path);

  getContentName(data->name, path);
  data->size = isFile ? pathStat.st_size : 0;
}

inline void getContentName(char* contentName, const char* path) {
  uint ind = 0, nameStartInd = 0;

  do {
    char currentChar = path[ind];

    if(currentChar == PATH_SEPARATOR) nameStartInd = ind + 1;
    else contentName[ind - nameStartInd] = currentChar;
  } while(path[ind++] != '\0');
}
