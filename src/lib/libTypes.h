#ifndef LIB_TYPES_H
#define LIB_TYPES_H

#include <pthread.h>
#include <stdlib.h>

#include "constants.h"
#include "shortcutTypes.h"

typedef enum { UNINITIALIZED, READABLE, EMPTY } BufferStatus;

typedef struct {
  const char* cwd;
  const char* archivePath;
  const char** contentPaths;
  size_t contentsQuantity;
} WstParams;

typedef struct ContentData {
  char* name;
  size_t size;
} ContentData;

typedef struct {
  byte data[BUFFER_MAX_SIZE];
  size_t size;
  BufferStatus status;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Buffer;

#endif