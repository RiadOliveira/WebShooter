#include "bufferHandler.h"

inline void initializeBuffers(Buffer* buffers, size_t quantity) {
  for(size_t ind = 0; ind < quantity; ind++) {
    Buffer* currentBuffer = &buffers[ind];

    currentBuffer->size = currentBuffer->consumedSize = 0;
    currentBuffer->status = UNSET;
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

inline byte getBufferCurrentByte(Buffer* buffer) {
  return buffer->data[buffer->consumedSize];
}

void consumeBuffersBytes(
  byte* dest, Buffer* buffers, uint* bufferInd, size_t bytesQuantity
) {
  Buffer* currentBuffer = &buffers[*bufferInd];
  const size_t consumedSize = currentBuffer->consumedSize;
  const size_t maxSizeConsumable = currentBuffer->size - consumedSize;

  size_t quantityToConsume = min(bytesQuantity, maxSizeConsumable);
  memcpy(dest, &currentBuffer->data[consumedSize], quantityToConsume);

  const bool bufferFullyConsumed = quantityToConsume == maxSizeConsumable;
  if(bufferFullyConsumed) {
    setBufferStatusAndWaitForNext(buffers, bufferInd, UNSET);

    currentBuffer = &buffers[*bufferInd];
    quantityToConsume = bytesQuantity - maxSizeConsumable;
    memcpy(&dest[maxSizeConsumable], currentBuffer->data, quantityToConsume);
  }

  currentBuffer->consumedSize += quantityToConsume;
}

inline void finishBuffersReading(Buffer* buffers, uint* bufferInd) {
  const bool currentHasData = buffers[*bufferInd].size > 0;
  if(currentHasData) {
    setBufferStatusAndWaitForNext(buffers, bufferInd, CONSUMABLE);
  }

  setBufferStatus(&buffers[*bufferInd], FINISHED);
}

inline void setBufferStatusAndWaitForNext(
  Buffer* buffers, uint* bufferInd, BufferStatus status
) {
  setBufferStatus(&buffers[*bufferInd], status);

  if(++(*bufferInd) == BUFFERS_QUANTITY) *bufferInd = 0;
  waitForBufferStatusMismatch(&buffers[*bufferInd], status);
}

inline void setBufferStatus(Buffer* buffer, BufferStatus status) {
  buffer->status = status;
  pthread_cond_signal(&buffer->cond);
}

inline void waitForBufferStatusMismatch(Buffer* buffer, BufferStatus status) {
  pthread_mutex_t* mutex = &buffer->mutex;

  pthread_mutex_lock(mutex);
  while(buffer->status == status) pthread_cond_wait(&buffer->cond, mutex);
  pthread_mutex_unlock(mutex);
}
