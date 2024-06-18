#include "contentHandler.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

inline void fillContentData(ContentData* data, const char* path) {
  getContentName(data->name, path);
  data->size = getContentSize(path);
}

inline void getContentName(char* name, const char* path) {
  int ind = -1, nameStartInd = 0;

  while(path[++ind] != NULL_TERMINATOR) {
    const char currentChar = path[ind];
    const int nameInd = ind - nameStartInd;

    if(currentChar != PATH_SEPARATOR) name[nameInd] = currentChar;
    else {
      const int nextInd = ind + 1;
      if(path[nextInd] == NULL_TERMINATOR) name[nameInd] = NULL_TERMINATOR;
      else nameStartInd = nextInd;
    }
  }
}

inline void concatPathSeparatorToFolderName(char* name) {
  const size_t length = strlen(name);
  name[length] = PATH_SEPARATOR;
  name[length + 1] = NULL_TERMINATOR;
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

inline bool isEmptySubContent(char* subContentName) {
  char* currentChar = subContentName;
  while(*currentChar == DOT_CHAR) currentChar++;

  return *currentChar == NULL_TERMINATOR;
}