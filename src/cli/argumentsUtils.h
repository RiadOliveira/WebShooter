#ifndef ARGUMENTS_UTILS_H
#define ARGUMENTS_UTILS_H

#include "argumentsParsing.h"
#include "errorPrinting.h"

bool printErrorIfInvalidArguments(ParsedArguments* arguments);
void printHelpMenu();

void freeArguments(ParsedArguments* arguments);

#endif