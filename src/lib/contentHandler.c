#include "contentHandler.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

inline void fillContentData(ContentData* data, const char* path) {
  getContentName(data->name, path);
  setContentMetadata(&data->metadata, path);
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

inline void setContentMetadata(Metadata* metadata, const char* path) {
  struct stat pathStat;
  if(stat(path, &pathStat) != 0) exitWithMessage(LOCATING_ERROR_MESSAGE, path);

  const uint stMode = pathStat.st_mode;
  const bool file = S_ISREG(stMode);
  const bool folder = S_ISDIR(stMode);
  if(!file && !folder) exitWithMessage(LOCATING_ERROR_MESSAGE, path);

  metadata->size = file ? pathStat.st_size : 0;
  metadata->mode = stMode;
  metadata->atime = pathStat.st_atime;
  metadata->mtime = pathStat.st_mtime;
}

inline void concatPathSeparatorToFolderName(char* name) {
  const size_t length = strlen(name);
  name[length] = PATH_SEPARATOR;
  name[length + 1] = NULL_TERMINATOR;
}

inline bool isFolder(ContentData* data) { return data->metadata.size == 0; }
inline bool isFile(ContentData* data) { return data->metadata.size > 0; }

inline bool isEmptySubContent(char* subContentName) {
  char* currentChar = subContentName;
  while(*currentChar == DOT_CHAR) currentChar++;

  return *currentChar == NULL_TERMINATOR;
}