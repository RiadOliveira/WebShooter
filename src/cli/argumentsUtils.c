#include "argumentsUtils.h"

const char* USAGE_INSTRUCTIONS_GENERAL = "Usage: wst [options] <arguments>\n";
const char* USAGE_INSTRUCTIONS_WEB =
  "wst -w <output_file> <content1> <content2> ...\n";
const char* USAGE_INSTRUCTIONS_UNWEB = "wst -u <file_to_unweb>\n";
const char* HELP_MENU =
  "Usage: wst [options] <arguments>\n"
  "Description: WebShooter is a tool to web files and folders into or "
  "unweb from a single archive.\n\n"
  "Options:\n"
  "  -w, --web\tWeb files or folders into a single archive.\n"
  "\t\tRequires an output file and one or more contents (files or folders).\n"
  "\t\tUsage: wst -w <output_file> <content1> <content2> ...\n\n"
  "  -u, --unweb\tUnweb a previously webbed archive into its original "
  "contents.\n"
  "\t\tRequires an input file to be unwebbed.\n"
  "\t\tUsage: wst -u <file_to_unweb>\n\n"
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
          "Insufficient contents passed to perform Web operation! At least "
          "two contents are required: <output_file> <content>\n\nUsage: %s",
          USAGE_INSTRUCTIONS_WEB
        );
      }

      return !hasOutputFileAndAtLeastOneContent;
    }
    case UNWEB: {
      bool hasFileToUnweb = quantityOfContents >= 1;
      if(!hasFileToUnweb) {
        printErrorMessage(
          "Insufficient contents passed to perform Unweb operation! At least "
          "one content is required: <file_to_unweb>\n\nUsage: %s",
          USAGE_INSTRUCTIONS_UNWEB
        );
      }

      return !hasFileToUnweb;
    }
    default: return false;
  }
}

inline void printHelpMenu() { printf("%s", HELP_MENU); }

inline void freeArguments(ParsedArguments* arguments) {
  free(arguments->contents);
  free(arguments);
}