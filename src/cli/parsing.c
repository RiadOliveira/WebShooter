#include "parsing.h"

const char OPTION_PREFIX = '-';

void initializeArguments(ParsedArguments*, int);
bool handleOption(char*, Option*);

void parseArguments(ParsedArguments* arguments, int argc, char** argv) {
  initializeArguments(arguments, argc);

  char** currentContent = arguments->contents;
  bool earlyReturn = false;
  for(int ind = 1; !earlyReturn && ind < argc; ind++) {
    char* currentArgv = argv[ind];

    if(currentArgv[0] != OPTION_PREFIX) *currentContent++ = currentArgv;
    else earlyReturn = handleOption(currentArgv, &arguments->option);
  }
}

inline void initializeArguments(ParsedArguments* arguments, int argc) {
  const size_t contentsSize = max(0, argc - 2);

  arguments->option = EMPTY;
  arguments->contentsSize = contentsSize;
  arguments->contents = malloc(contentsSize * sizeof(char*));
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
