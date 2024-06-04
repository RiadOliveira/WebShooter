#include "argumentsUtils.h"

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

bool printErrorIfInvalidArguments(ParsedArguments* arguments) {
  uint quantityOfContents = arguments->quantityOfContents;

  switch(arguments->option) {
    case EMPTY: {
      printErrorMessage(
        "Invalid or no option received. For detailed usage instructions, try "
        "'-h' or '--help'.\n%s",
        USAGE_INSTRUCTIONS_GENERAL
      );
      return true;
    }
    case WEB: {
      bool hasOutputFileAndAtLeastOneContent = quantityOfContents >= 2;
      if(!hasOutputFileAndAtLeastOneContent) {
        printErrorMessage(
          "Insufficient arguments passed to perform Web operation! At least "
          "two arguments are required: <output_archive> <content>\n\n%s",
          USAGE_INSTRUCTIONS_WEB
        );
      }

      return !hasOutputFileAndAtLeastOneContent;
    }
    case UNWEB: {
      bool hasFileToUnweb = quantityOfContents >= 1;
      if(!hasFileToUnweb) {
        printErrorMessage(
          "Insufficient arguments passed to perform Unweb operation! At least "
          "one argument is required: <archive_to_unweb>\n\n%s",
          USAGE_INSTRUCTIONS_UNWEB
        );
      }

      return !hasFileToUnweb;
    }
    default: return false;
  }
}

inline void printHelpMenu() {
  printf(
    HELP_MENU, USAGE_INSTRUCTIONS_GENERAL, USAGE_INSTRUCTIONS_WEB,
    USAGE_INSTRUCTIONS_UNWEB
  );
}

inline void freeArguments(ParsedArguments* arguments) {
  free(arguments->contents);
  free(arguments);
}