#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H

#include <pthread.h>

#include "shortcutTypes.h"

typedef enum { UNINITIALIZED, READABLE, EMPTY } BufferStatus;

#define BUFFER_MAX_SIZE 1024 * 1000
#define BUFFERS_QUANTITY 2

typedef struct {
  byte data[BUFFER_MAX_SIZE];
  size_t size;
  BufferStatus status;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Buffer;

void initializeBuffers(Buffer* buffers, size_t quantity);
void advanceBufferAndWait(Buffer* buffers, uint* bufferInd);
void finalizeBuffers(Buffer* buffers, size_t quantity);

#endif