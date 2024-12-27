#ifndef CLI_TYPES
#define CLI_TYPES

#include <stdlib.h>

#include "shortcutTypes.h"

typedef enum { WEB = 'w', UNWEB = 'u', HELP = 'h', UNSET } Option;

typedef struct {
  Option option;
  char** contents;
  size_t contentsQuantity;
} ParsedArguments;

#endif