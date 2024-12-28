#include "error.h"

inline void exitWithError(const char* format, ...) {
  va_list messages;

  va_start(messages, format);
  vfprintf(stderr, format, messages);
  fprintf(stderr, "\n");
  va_end(messages);

  exit(EXIT_FAILURE);
}