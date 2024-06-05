#include "validation.h"

const char* USAGE_INSTRUCTIONS_GENERAL = "Usage: wst [options] <arguments>\n";
const char* USAGE_INSTRUCTIONS_WEB =
  "Usage: wst -w <output_archive> <content1> <content2> ...\n";
const char* USAGE_INSTRUCTIONS_UNWEB =
  "Usage: wst -u <archive_to_unweb1> <archive_to_unweb2> ...\n";
const char* HELP_MENU =
  "%sDescription: WebShooter is a tool to web files and folders into or "
  "unweb from a single archive.\n\n"
  "Options:\n"
  "  -w, --web\tWeb files or folders into a single archive.\n"
  "\t\tRequires an output file and one or more contents (files or folders).\n"
  "\t\t%s\n"
  "  -u, --unweb\tUnweb previously webbed archives into their original "
  "contents.\n"
  "\t\tRequires one or more input archives to be unwebbed.\n"
  "\t\t%s\n"
  "  -h, --help\tShow this help message.\n";

bool validateArguments(ParsedArguments* arguments) {
  const size_t contentsSize = arguments->contentsSize;

  switch(arguments->option) {
    case EMPTY: {
      printErrorMessage(
        "Invalid or no option received. For detailed usage instructions, try "
        "'-h' or '--help'.\n%s",
        USAGE_INSTRUCTIONS_GENERAL
      );
      return false;
    }
    case WEB: {
      const bool hasOutputFileAndAtLeastOneContent = contentsSize >= 2;
      if(!hasOutputFileAndAtLeastOneContent) {
        printErrorMessage(
          "Insufficient arguments passed to perform Web operation! At least "
          "two arguments are required: <output_archive> <content>\n\n%s",
          USAGE_INSTRUCTIONS_WEB
        );
      }

      return hasOutputFileAndAtLeastOneContent;
    }
    case UNWEB: {
      const bool hasFileToUnweb = contentsSize >= 1;
      if(!hasFileToUnweb) {
        printErrorMessage(
          "Insufficient arguments passed to perform Unweb operation! At least "
          "one argument is required: <archive_to_unweb>\n\n%s",
          USAGE_INSTRUCTIONS_UNWEB
        );
      }

      return hasFileToUnweb;
    }
    default: return true;
  }
}

inline void printHelpMenu() {
  printf(
    HELP_MENU, USAGE_INSTRUCTIONS_GENERAL, USAGE_INSTRUCTIONS_WEB,
    USAGE_INSTRUCTIONS_UNWEB
  );
}
