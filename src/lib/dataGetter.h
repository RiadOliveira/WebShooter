#ifndef DATA_GETTER_H
#define DATA_GETTER_H

#include <sys/stat.h>

#include "constants.h"
#include "errorThrower.h"
#include "libTypes.h"
#include "pathHandler.h"

#if defined(_WIN32) || defined(_WIN64)
  #define stat _stat
#endif

void getContentData(ContentData* data, const char* path);

#endif