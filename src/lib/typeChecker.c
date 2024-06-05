#include "typeChecker.h"

const char* LOCATING_ERROR_MESSAGE =
  "Error while locating the content identified by the path: %s\n";

ContentType checkContentType(const char* cwd, const char* content) {
  const char* path = joinPaths(cwd, content);

#if defined(_WIN32) || defined(_WIN64)
  DWORD attributes = GetFileAttributes(path);
  if(attributes == INVALID_FILE_ATTRIBUTES) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, path);
  }
  return attributes & FILE_ATTRIBUTE_DIRECTORY ? C_FOLDER : C_FILE;
#else
  struct stat pathStat;
  if(lstat(path, &pathStat) != 0) {
    printErrorAndExit(LOCATING_ERROR_MESSAGE, path);
  }

  const uint stMode = pathStat.st_mode;
  const bool isFolder = S_ISDIR(stMode);
  const bool isFile = S_ISREG(stMode);

  if(!isFolder && !isFile) printErrorAndExit(LOCATING_ERROR_MESSAGE, path);
  return isFolder;
#endif

  free(path);
}