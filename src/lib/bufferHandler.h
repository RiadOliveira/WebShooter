#ifndef BUFFER_HANDLER
#define BUFFER_HANDLER

#include <pthread.h>
#include <string.h>

#include "mathUtils.h"
#include "shortcutTypes.h"

#define BUFFER_MAX_SIZE 1024 * 1024
#define BUFFERS_QUANTITY 2

typedef enum { UNSET, CONSUMABLE, FINISHED } BufferStatus;

typedef struct {
  byte data[BUFFER_MAX_SIZE];
  size_t size, consumedSize;
  BufferStatus status;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Buffer;

void initializeBuffers(Buffer* buffers, size_t quantity);
void finalizeBuffers(Buffer* buffers, size_t quantity);

byte getBufferCurrentByte(Buffer* buffer);
void consumeBuffersBytes(
  byte* dest, Buffer* buffers, uint* bufferInd, size_t bytesQuantity
);

void finishBuffersReading(Buffer* buffers, uint* bufferInd);
void unlockCurrentBufferToGetNextLocked(
  Buffer* buffers, uint* bufferInd, BufferStatus status
);
void unlockBuffer(Buffer* buffer, BufferStatus status);
void lockBufferWhenStatusMismatch(Buffer* buffer, BufferStatus status);

#endif