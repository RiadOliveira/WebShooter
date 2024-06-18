#include "fileManager.h"

inline FILE* openFileOrExit(const char* path, const char* modes) {
  FILE* file = fopen(path, modes);
  if(file == NULL || *path == PATH_SEPARATOR) {
    exitWithMessage("Error opening the file indicated by the path: %s", path);
  }

  return file;
}

inline DIR* openFolderOrExit(const char* path) {
  DIR* folder = opendir(path);
  if(folder == NULL || *path == PATH_SEPARATOR) {
    exitWithMessage("Error opening the folder indicated by the path: %s", path);
  }

  return folder;
}

inline void createFolder(const char* path) {
  struct stat pathStat;
  if(stat(path, &pathStat) == 0) return;

  if(mkdir(path, 0777) == 0) return;
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
