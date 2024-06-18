#include "bufferHandler.h"

inline void initializeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    currentBuffer->size = currentBuffer->status = UNINITIALIZED;
    pthread_mutex_init(&currentBuffer->mutex, NULL);
    pthread_cond_init(&currentBuffer->cond, NULL);
  }
}

void advanceBufferAndWait(Buffer* buffers, uint* bufferInd) {
  Buffer* selectedBuffer = &buffers[*bufferInd];
  selectedBuffer->status = READABLE;
  pthread_cond_signal(&selectedBuffer->cond);

  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;
  selectedBuffer = &buffers[*bufferInd];
  pthread_mutex_t* mutex = &selectedBuffer->mutex;

  pthread_mutex_lock(mutex);
  while(selectedBuffer->status != UNINITIALIZED) {
    pthread_cond_wait(&selectedBuffer->cond, mutex);
  }
  pthread_mutex_unlock(mutex);
}

inline void finalizeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    pthread_mutex_destroy(&currentBuffer->mutex);
    pthread_cond_destroy(&currentBuffer->cond);
  }
}