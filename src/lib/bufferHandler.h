#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H

#include <pthread.h>

#include "shortcutTypes.h"

#define BUFFER_MAX_SIZE 1024 * 1000
#define BUFFERS_QUANTITY 2

typedef enum { UNINITIALIZED, READABLE, EMPTY } BufferStatus;

typedef struct {
  byte data[BUFFER_MAX_SIZE];
  size_t size, consumedSize;
  BufferStatus status;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Buffer;

void initializeBuffers(Buffer* buffers, size_t quantity);
void finalizeBuffers(Buffer* buffers, size_t quantity);

void advanceBufferAndWaitForNext(Buffer* buffers, uint* bufferInd);
void waitBufferReachStatus(Buffer* buffer, BufferStatus status);

#endif