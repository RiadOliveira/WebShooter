#include "libwst.h"
#include "parser.h"
#include "validator.h"

void fillParams(WstParams*, ParsedArguments*);

int main(int argc, char** argv) {
  ParsedArguments arguments;
  parseArguments(&arguments, argc, argv);
  validateArguments(&arguments);

  WstParams params;
  fillParams(&params, &arguments);

  switch(arguments.option) {
    case WEB: webContentsIntoArchive(&params); break;
    case UNWEB: unwebArchiveIntoContents(&params); break;
    default: printHelpMenu();
  }

  free(arguments.contents);
  return EXIT_SUCCESS;
}

inline void fillParams(WstParams* params, ParsedArguments* arguments) {
  params->archivePath = *arguments->contents;
  params->contentsQuantity = arguments->contentsQuantity - 1;

  const bool noContentsPassed = params->contentsQuantity == 0;
  params->contentPaths =
    noContentsPassed ? NULL : (const char**)&arguments->contents[1];
}
