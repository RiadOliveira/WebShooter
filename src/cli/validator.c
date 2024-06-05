#include "validator.h"

const char* USAGE_INSTRUCTIONS_GENERAL = "Usage: wst [options] <arguments>\n";
const char* USAGE_INSTRUCTIONS_WEB =
  "Usage: wst -w <output_archive_path> <content_path1> <content_path2> ...\n";
const char* USAGE_INSTRUCTIONS_UNWEB =
  "Usage: wst -u <archive_to_unweb_path1> <archive_to_unweb_path2> ...\n";
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

void validateArguments(ParsedArguments* arguments) {
  const size_t contentsSize = arguments->contentsSize;

  switch(arguments->option) {
    case EMPTY: {
      printErrorAndExit(
        "Invalid or no option received. For detailed usage instructions, try "
        "'-h' or '--help'.\n%s",
        USAGE_INSTRUCTIONS_GENERAL
      );
    }
    case WEB: {
      if(contentsSize >= 2) break;
      printErrorAndExit(
        "Insufficient arguments passed to perform Web operation! At least "
        "two arguments are required: <output_archive_path> "
        "<content_path>\n\n%s",
        USAGE_INSTRUCTIONS_WEB
      );
    }
    case UNWEB: {
      if(contentsSize >= 1) break;
      printErrorAndExit(
        "Insufficient arguments passed to perform Unweb operation! At least "
        "one argument is required: <archive_to_unweb_path>\n\n%s",
        USAGE_INSTRUCTIONS_UNWEB
      );
    }
    default: break;
  }
}

inline void printHelpMenu() {
  printf(
    HELP_MENU, USAGE_INSTRUCTIONS_GENERAL, USAGE_INSTRUCTIONS_WEB,
    USAGE_INSTRUCTIONS_UNWEB
  );
}
