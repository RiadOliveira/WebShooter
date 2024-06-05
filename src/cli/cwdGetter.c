#include "cwdGetter.h"

inline void getCwdOrExit(char* cwd, size_t size) {
  if(getcwd(cwd, size) != NULL) return;

  printErrorAndExit(
    "[Internal Error] Failed to get the current working directory\n"
  );
}