#ifndef ARGUMENTS_PARSING_H
#define ARGUMENTS_PARSING_H

#include <stdio.h>
#include <stdlib.h>

#include "shortcutTypes.h"

typedef enum { WEB = 'w', UNWEB = 'u', HELP = 'h', EMPTY } Option;

typedef struct ArgumentContent {
  char* content;
  struct ArgumentContent* next;
} ArgumentContent;

typedef struct {
  Option option;
  ArgumentContent* argumentContent;
} ParsedArguments;

ParsedArguments* parseArguments(int argc, char** argv);

#endif