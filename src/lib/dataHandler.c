#include "dataHandler.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

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
  getContentName(data->name, path);
  data->size = getContentSize(path);
}

inline void getContentName(char* contentName, const char* path) {
  uint ind = 0, nameStartInd = 0;

  do {
    char currentChar = path[ind];

    if(currentChar == *PATH_SEPARATOR) nameStartInd = ind + 1;
    else contentName[ind - nameStartInd] = currentChar;
  } while(path[ind++] != '\0');
}

inline size_t getContentSize(const char* path) {
  struct stat pathStat;
  if(stat(path, &pathStat) != 0) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, path);
  }

  const uint stMode = pathStat.st_mode;
  const bool file = S_ISREG(stMode);
  const bool folder = S_ISDIR(stMode);
  if(!file && !folder) printErrorAndExit(LOCATING_ERROR_MESSAGE, path);

  return file ? pathStat.st_size : 0;
}

inline bool isFolder(ContentData* data) { return data->size == 0; }