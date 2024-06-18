#include "cwdGetter.h"
#include "libwst.h"
#include "parser.h"
#include "validator.h"

void fillParams(WstParams*, ParsedArguments*);

int main(int argc, char** argv) {
  ParsedArguments arguments;
  parseArguments(&arguments, argc, argv);
  validateArguments(&arguments);

  char cwd[PATH_MAX];
  getCwdOrExit(cwd, PATH_MAX);

  WstParams params = {cwd};
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
  params->contentPaths = (const char**)&arguments->contents[1];
  params->contentsQuantity = arguments->contentsQuantity - 1;
}
