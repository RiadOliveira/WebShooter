#ifndef ARGUMENTS_VALIDATION_H
#define ARGUMENTS_VALIDATION_H

#include "argumentsParsing.h"
#include "errorPrinting.h"

bool printErrorIfInvalidArguments(ParsedArguments* arguments);
void printHelpMenu();

void freeArguments(ParsedArguments* parsedArguments);

#endif