#include "libwst.h"
#include "parsing.h"
#include "validation.h"

bool getCwdOrPrintError(char*, size_t);
void fillParams(WstOperationParams*, ParsedArguments*);

int main(int argc, char** argv) {
  ParsedArguments arguments;
  parseArguments(&arguments, argc, argv);
  if(!validateArguments(&arguments)) return EXIT_FAILURE;

  char cwd[PATH_MAX];
  if(!getCwdOrPrintError(cwd, PATH_MAX)) return EXIT_FAILURE;

  WstOperationParams params = {cwd};
  fillParams(&params, &arguments);

  switch(arguments.option) {
    case WEB: webContentsIntoArchive(&params, arguments.contents[0]); break;
    case UNWEB: unwebArchiveIntoContents(&params); break;
    default: printHelpMenu();
  }

  free(arguments.contents);
  return EXIT_SUCCESS;
}

inline bool getCwdOrPrintError(char* cwd, size_t size) {
  const bool cwdObtained = getcwd(cwd, size) != NULL;
  if(!cwdObtained) {
    printErrorMessage(
      "[Internal Error] Failed to get the current working directory\n"
    );
  }

  return cwdObtained;
}

inline void fillParams(WstOperationParams* params, ParsedArguments* arguments) {
  const bool webOption = arguments->option == WEB;

  params->archiveOrContentPaths = (const char**)&arguments->contents[webOption];
  params->archivesOrContentsSize = arguments->contentsSize - webOption;
}
