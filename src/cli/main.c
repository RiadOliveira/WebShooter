#include "argumentsParsing.h"
#include "argumentsValidation.h"

int main(int argc, char** argv) {
  ParsedArguments* arguments = parseArguments(argc, argv);
  if(printErrorIfInvalidArguments(arguments)) return EXIT_FAILURE;

  if(arguments->option == HELP) printHelpMenu();
  else {
  }

  freeArguments(arguments);
  return EXIT_SUCCESS;
}
