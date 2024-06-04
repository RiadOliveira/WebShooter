#include "argumentsParsing.h"

const char OPTION_PREFIX = '-';

bool handleOption(char* currentArgv, Option* option);
void handleContent(
  char* currentArgv, ArgumentContent** currentContent, bool lastArgv
);

ParsedArguments* parseArguments(int argc, char** argv) {
  ParsedArguments* parsedArguments = malloc(sizeof(ParsedArguments));
  parsedArguments->argumentContent = malloc(sizeof(ArgumentContent));
  parsedArguments->option = EMPTY;

  ArgumentContent* currentContent = parsedArguments->argumentContent;
  currentContent->content = NULL, currentContent->next = NULL;

  bool earlyReturn = false;
  for(int ind = 1; !earlyReturn && ind < argc; ind++) {
    char* currentArgv = argv[ind];

    if(currentArgv[0] == OPTION_PREFIX) {
      earlyReturn = handleOption(currentArgv, &parsedArguments->option);
    } else {
      bool lastArgv = ind == argc - 1;
      handleContent(currentArgv, &currentContent, lastArgv);
    }
  }

  return parsedArguments;
}

inline bool handleOption(char* currentArgv, Option* option) {
  bool doubleOptionPrefix = currentArgv[1] == OPTION_PREFIX;
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

inline void handleContent(
  char* currentArgv, ArgumentContent** currentContent, bool lastArgv
) {
  ArgumentContent* current = *currentContent;
  current->content = currentArgv;
  if(lastArgv) return;

  current->next = malloc(sizeof(ArgumentContent));
  *currentContent = current = current->next;
  current->content = NULL, current->next = NULL;
}
