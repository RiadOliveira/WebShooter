#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H

#include <pthread.h>
#include <string.h>

#include "mathUtils.h"
#include "shortcutTypes.h"

#define BUFFER_MAX_SIZE 1024 * 1000
#define BUFFERS_QUANTITY 2

typedef enum { UNSET, READABLE, FINISHED } BufferStatus;

typedef struct {
  byte data[BUFFER_MAX_SIZE];
  size_t size, consumedSize;
  BufferStatus status;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Buffer;

void initializeBuffers(Buffer* buffers, size_t quantity);
void finalizeBuffers(Buffer* buffers, size_t quantity);

void consumeBuffersBytes(
  byte* dest, Buffer* buffers, uint* bufferInd, size_t bytesQuantity
);

uint getIndOfFirstBufferWithStatus(Buffer* buffers, BufferStatus status);
void setBufferStatusAndWaitForNext(
  BufferStatus status, Buffer* buffers, uint* bufferInd
);
void waitForBufferStatusMismatch(Buffer* buffer, BufferStatus status);
void finishBuffersReading(Buffer* buffers, uint* bufferInd);

#endif