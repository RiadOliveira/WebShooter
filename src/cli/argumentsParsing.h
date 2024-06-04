#ifndef ARGUMENTS_PARSING_H
#define ARGUMENTS_PARSING_H

#include <stdio.h>
#include <stdlib.h>

#include "mathUtils.h"
#include "shortcutTypes.h"

typedef enum { WEB = 'w', UNWEB = 'u', HELP = 'h', EMPTY } Option;

typedef struct {
  Option option;
  char** contents;
  uint quantityOfContents;
} ParsedArguments;

ParsedArguments* parseArguments(int argc, char** argv);

#endif