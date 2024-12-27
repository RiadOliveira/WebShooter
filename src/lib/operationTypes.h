#ifndef OPERATION_TYPES
#define OPERATION_TYPES

#include "bufferHandler.h"
#include "contentHandler.h"

typedef struct {
  Buffer* buffers;
  uint bufferInd;
  ContentData contentData;
  char fullPath[PATH_MAX_SIZE];
} WebbingOperationData;

#endif