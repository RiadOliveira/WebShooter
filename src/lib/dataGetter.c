#include "dataGetter.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

inline void getContentName(char* contentName, const char* path) {
  unsigned int ind = 0, nameStartInd = 0;

  do {
    char currentChar = path[ind];

    if(currentChar == PATH_SEPARATOR) nameStartInd = ind + 1;
    else contentName[ind - nameStartInd] = currentChar;
  } while(path[ind++] != '\0');
}

inline ullong getContentSize(const char* contentPath) {
  struct stat pathStat;
  if(stat(contentPath, &pathStat) != 0) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, contentPath);
  }

  const uint stMode = pathStat.st_mode;
  const bool isFile = S_ISREG(stMode);
  const bool isFolder = S_ISDIR(stMode);
  if(!isFile && !isFolder) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, contentPath);
  }

  return isFile ? pathStat.st_size : 0;
}
