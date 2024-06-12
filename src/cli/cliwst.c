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
    case WEB: webContentsIntoArchive(&params, *arguments.contents); break;
    case UNWEB: unwebArchiveIntoContents(&params); break;
    default: printHelpMenu();
  }

  free(arguments.contents);
  return EXIT_SUCCESS;
}

inline void fillParams(WstParams* params, ParsedArguments* arguments) {
  const bool webOption = arguments->option == WEB;

  params->contentOrArchivePaths = (const char**)&arguments->contents[webOption];
  params->contentsOrArchivesQuantity = arguments->contentsQuantity - webOption;
}
