#include "errorPrinting.h"

inline void printErrorMessage(const char* format, ...) {
  va_list messages;

  va_start(messages, format);
  vfprintf(stderr, format, messages);
  va_end(messages);
}