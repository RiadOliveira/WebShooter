#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "errorThrower.h"
#include "libTypes.h"
#include "pathHandler.h"

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
#else
  #include <sys/stat.h>
#endif

ContentType checkContentType(const char* cwd, const char* content);

#endif