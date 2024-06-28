#include "fileManager.h"

inline FILE* openFileOrExit(const char* path, const char* modes) {
  FILE* file = fopen(path, modes);
  if(file == NULL || *path == PATH_SEPARATOR) {
    exitWithMessage("Error opening the following file: %s", path);
  }

  return file;
}

inline DIR* openFolderOrExit(const char* path) {
  DIR* folder = opendir(path);
  if(folder == NULL || *path == PATH_SEPARATOR) {
    exitWithMessage("Error opening the following folder: %s", path);
  }

  return folder;
}

inline void setFileOrFolderMetadata(const char* path, Metadata* metadata) {
  struct utimbuf timesData = {metadata->atime, metadata->mtime};

  bool successfullySet = chmod(path, metadata->mode) != -1;
  successfullySet &= chown(path, metadata->uid, metadata->gid) != -1;
  successfullySet &= utime(path, &timesData) != -1;

  if(successfullySet) return;
  exitWithMessage("Error setting following content metadata: %s", path);
}

inline void createFolder(const char* path) {
  struct stat pathStat;

  bool successfullyCreated = stat(path, &pathStat) == 0;
  successfullyCreated |= mkdir(path, DEFAULT_FOLDER_PERMISSIONS) == 0;

  if(successfullyCreated) return;
  exitWithMessage("Error creating the folder indicated by the path: %s", path);
}

inline void appendPath(
  char* destination, unsigned int position, const char* source
) {
  bool destinationHasSeparator = destination[position - 1] == PATH_SEPARATOR;
  unsigned int parsedPosition = position;

  if(!destinationHasSeparator) destination[parsedPosition++] = PATH_SEPARATOR;
  destination[parsedPosition] = NULL_TERMINATOR;

  bool sourceHasSeparator = *source == PATH_SEPARATOR;
  strcat(&destination[parsedPosition], &source[sourceHasSeparator]);
}
