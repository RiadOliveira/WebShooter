#include "fileHandler.h"

inline FILE* openFileOrExit(const char* path, const char* modes) {
  FILE* file = fopen(path, modes);
  if(file == NULL) {
    printErrorAndExit("Error opening the file indicated by the path: %s", path);
  }

  return file;
}