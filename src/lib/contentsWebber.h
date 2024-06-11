#ifndef CONTENTS_WEBBER_H
#define CONTENTS_WEBBER_H

#include <pthread.h>

#include "dataGetter.h"
#include "fileHandler.h"
#include "libwst.h"

typedef struct {
  WstParams* wstParams;
  Buffer* buffers;
  char* currentFullPath;
  const size_t cwdLength;
} ReadThreadParams;

typedef struct {
  FILE* archive;
  Buffer* buffers;
} WriteThreadParams;

#endif
