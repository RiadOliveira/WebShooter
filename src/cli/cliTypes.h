#ifndef CLI_TYPES_H
#define CLI_TYPES_H

#include <stdlib.h>

#include "shortcutTypes.h"

typedef enum { WEB = 'w', UNWEB = 'u', HELP = 'h', EMPTY } Option;

typedef struct {
  Option option;
  char** contents;
  size_t contentsQuantity;
} ParsedArguments;

#endif