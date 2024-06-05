#include "pathHandler.h"

const char* PATH_SEPARATOR = "/";
const int SEPARATOR_AND_TERMINATOR_TOTAL_SIZE = 2;

const char* joinPaths(const char* first, const char* second) {
  char* result = malloc(
    strlen(first) + strlen(second) + SEPARATOR_AND_TERMINATOR_TOTAL_SIZE
  );

  strcpy(result, first);
  strcat(result, PATH_SEPARATOR);
  strcat(result, second);

  return result;
}