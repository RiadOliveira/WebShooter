#include "validator.h"

const char* USAGE_INSTRUCTIONS_GENERAL = "Usage: wst [options] <arguments>\n";
const char* USAGE_INSTRUCTIONS_WEB =
  "Usage: wst -w <output_archive_path> <content_path1> [content_path2] ...\n";
const char* USAGE_INSTRUCTIONS_UNWEB =
  "Usage: wst -u <archive_to_unweb_path> [contents_output_path]\n";
const char* HELP_MENU =
  "%sDescription: WebShooter is a tool to web files and folders into or "
  "unweb from a single archive.\n\n"
  "Options:\n"
  "  -w, --web\tWeb files or folders into a single archive.\n"
  "\t\tRequires an output archive path and at least one content (file or "
  "folder).\n"
  "\t\t%s\n"
  "  -u, --unweb\tUnweb a previously webbed archive into its original "
  "contents.\n"
  "\t\tRequires an input archive. If no output path is "
  "provided, the current path is used.\n"
  "\t\t%s\n"
  "  -h, --help\tShow this help message.\n";

void validateArguments(ParsedArguments* arguments) {
  const size_t contentsQuantity = arguments->contentsQuantity;

  switch(arguments->option) {
    case UNSET: {
      exitWithMessage(
        "Invalid or no option received. For detailed usage instructions, try "
        "'-h' or '--help'.\n%s",
        USAGE_INSTRUCTIONS_GENERAL
      );
    }
    case WEB: {
      if(contentsQuantity >= 2) break;
      exitWithMessage(
        "Insufficient arguments passed to perform Web operation! At least "
        "two arguments are required: <output_archive_path> "
        "<content_path>\n\n%s",
        USAGE_INSTRUCTIONS_WEB
      );
    }
    case UNWEB: {
      if(contentsQuantity >= 1) break;
      exitWithMessage(
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
