#ifndef OPERATION_TYPES_H
#define OPERATION_TYPES_H

#include "bufferHandler.h"
#include "contentHandler.h"

typedef struct {
  Buffer* buffers;
  uint bufferInd;
  ContentData contentData;
  char fullPath[PATH_MAX_SIZE];
} WebbingOperationData;

#endif