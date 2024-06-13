#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <pthread.h>
#include <sys/stat.h>

#include "constants.h"
#include "errorThrower.h"
#include "libTypes.h"
#include "pathHandler.h"

#if defined(_WIN32) || defined(_WIN64)
  #define stat _stat
#endif

void initializeBuffers(Buffer* buffers, size_t quantity);
void finalizeBuffers(Buffer* buffers, size_t quantity);

void fillContentData(ContentData* data, const char* path);
void getContentName(char* name, const char* path);
void concatPathSeparatorToFolderName(char* name);
size_t getContentSize(const char* path);

bool isFolder(ContentData* data);
bool isEmptySubContent(char* subContentName);

#endif