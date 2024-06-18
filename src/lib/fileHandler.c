#include "fileHandler.h"

inline FILE* openFileOrExit(const char* path, const char* modes) {
  FILE* file = fopen(path, modes);
  if(file == NULL || *path == PATH_SEPARATOR) {
    printErrorAndExit("Error opening the file indicated by the path: %s", path);
  }

  return file;
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
