#include "bufferHandler.h"

inline void initializeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    currentBuffer->size = currentBuffer->status = UNINITIALIZED;
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

inline void advanceBufferAndWaitForNext(Buffer* buffers, uint* bufferInd) {
  Buffer* selectedBuffer = &buffers[*bufferInd];
  selectedBuffer->status = READABLE;
  pthread_cond_signal(&selectedBuffer->cond);

  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;
  selectedBuffer = &buffers[*bufferInd];

  waitBufferReachStatus(selectedBuffer, UNINITIALIZED);
}

inline void waitBufferReachStatus(Buffer* buffer, BufferStatus status) {
  pthread_mutex_t* mutex = &buffer->mutex;

  pthread_mutex_lock(mutex);
  while(buffer->status != status) {
    pthread_cond_wait(&buffer->cond, mutex);
  }
  pthread_mutex_unlock(mutex);
}