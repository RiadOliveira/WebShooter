#include "contentHandler.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

inline void getContentData(ContentData* data, const char* path) {
  getContentName(data->name, path);
  getContentMetadata(&data->metadata, path);
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

inline void getContentMetadata(Metadata* metadata, const char* path) {
  struct stat pathStat;
  if(stat(path, &pathStat) != 0) exitWithMessage(LOCATING_ERROR_MESSAGE, path);

  const uint stMode = pathStat.st_mode;
  const bool file = S_ISREG(stMode);
  const bool folder = S_ISDIR(stMode);
  if(!file && !folder) exitWithMessage(LOCATING_ERROR_MESSAGE, path);

  metadata->mode = stMode;
  metadata->uid = pathStat.st_uid;
  metadata->gid = pathStat.st_gid;
  metadata->atime = pathStat.st_atime;
  metadata->mtime = pathStat.st_mtime;
  if(file) metadata->size = pathStat.st_size;
}

inline size_t getMetadataStructSize(Metadata* metadata) {
  const bool file = isFile(metadata);
  return file ? METADATA_SIZE : METADATA_SIZE_WITHOUT_SIZE_ATTRIBUTE;
}

inline bool isFile(Metadata* metadata) { return S_ISREG(metadata->mode); }
inline bool isFolder(Metadata* metadata) { return S_ISDIR(metadata->mode); }

inline bool isEmptySubContent(char* subContentName) {
  char* currentChar = subContentName;
  while(*currentChar == DOT_CHAR) currentChar++;

  return *currentChar == NULL_TERMINATOR;
}