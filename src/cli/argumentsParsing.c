#include "argumentsParsing.h"

const char OPTION_PREFIX = '-';

ParsedArguments* generateInitializedParsedArguments(int argc);
bool handleOption(char* currentArgv, Option* option);

ParsedArguments* parseArguments(int argc, char** argv) {
  ParsedArguments* parsedArguments = generateInitializedParsedArguments(argc);

  char** currentContent = parsedArguments->contents;
  bool earlyReturn = false;
  for(int ind = 1; !earlyReturn && ind < argc; ind++) {
    char* currentArgv = argv[ind];

    if(currentArgv[0] != OPTION_PREFIX) *currentContent++ = currentArgv;
    else earlyReturn = handleOption(currentArgv, &parsedArguments->option);
  }

  return parsedArguments;
}

inline ParsedArguments* generateInitializedParsedArguments(int argc) {
  ParsedArguments* parsedArguments = malloc(sizeof(ParsedArguments));
  const uint quantityOfContents = max(0, argc - 2);

  parsedArguments->option = EMPTY;
  parsedArguments->quantityOfContents = quantityOfContents;
  parsedArguments->contents = malloc(quantityOfContents * sizeof(char*));

  return parsedArguments;
}

inline bool handleOption(char* currentArgv, Option* option) {
  const bool doubleOptionPrefix = currentArgv[1] == OPTION_PREFIX;
  *option = currentArgv[1 + doubleOptionPrefix];

  switch(*option) {
    case WEB:
    case UNWEB: return false;
    case HELP: return true;
    default: {
      *option = EMPTY;
      return false;
    }
  }
}
