#include "bufferHandler.h"

inline void initializeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    currentBuffer->size = currentBuffer->consumedSize = 0;
    currentBuffer->status = UNINITIALIZED;
    pthread_mutex_init(&currentBuffer->mutex, NULL);
    pthread_cond_init(&currentBuffer->cond, NULL);
  }
}

inline void finalizeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    pthread_mutex_destroy(&currentBuffer->mutex);
    pthread_cond_destroy(&currentBuffer->cond);
  }
}

inline uint getIndOfFirstBufferWithStatus(
  Buffer* buffers, BufferStatus status
) {
  uint bufferInd = 0;
  while(buffers[bufferInd].status != UNINITIALIZED) bufferInd++;
  return bufferInd;
}

inline void setBufferStatusAndWaitForNext(
  BufferStatus status, Buffer* buffers, uint* bufferInd
) {
  Buffer* selectedBuffer = &buffers[*bufferInd];
  selectedBuffer->status = status;
  pthread_cond_signal(&selectedBuffer->cond);

  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;
  waitForBufferStatusMismatch(&buffers[*bufferInd], status);
}

inline void waitForBufferStatusMismatch(Buffer* buffer, BufferStatus status) {
  pthread_mutex_t* mutex = &buffer->mutex;

  pthread_mutex_lock(mutex);
  while(buffer->status == status) pthread_cond_wait(&buffer->cond, mutex);
  pthread_mutex_unlock(mutex);
}

inline void finishBuffersReading(Buffer* buffers, uint* bufferInd) {
  const bool currentBufferHasData = buffers[*bufferInd].size > 0;
  if(currentBufferHasData) {
    setBufferStatusAndWaitForNext(READABLE, buffers, bufferInd);
  }

  buffers[*bufferInd].status = EMPTY;
}