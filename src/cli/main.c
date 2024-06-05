#include "argumentsParsing.h"
#include "argumentsUtils.h"
#include "libwst.h"

int main(int argc, char** argv) {
  ParsedArguments* arguments = parseArguments(argc, argv);
  if(printErrorIfInvalidArguments(arguments)) return EXIT_FAILURE;

  const char* cwd = getCwdOrPrintError();
  if(cwd == NULL) return EXIT_FAILURE;

  const char** contents = (const char**)arguments->contents;
  switch(arguments->option) {
    case WEB: {
      uint contentsQuantityWithoutOutput = arguments->quantityOfContents - 1;
      webContentsIntoArchive(
        cwd, contents[0], &contents[1], contentsQuantityWithoutOutput
      );
      break;
    }
    case UNWEB: unwebArchiveIntoContents(cwd, contents[0]); break;
    default: printHelpMenu();
  }

  freeArguments(arguments);
  return EXIT_SUCCESS;
}
