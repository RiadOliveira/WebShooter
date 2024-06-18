#ifndef CONTENT_HANDLER_H
#define CONTENT_HANDLER_H

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "errorThrower.h"
#include "fileManager.h"
#include "shortcutTypes.h"

typedef struct ContentData {
  char* name;
  size_t size;
} ContentData;

void fillContentData(ContentData* data, const char* path);
void getContentName(char* name, const char* path);
void concatPathSeparatorToFolderName(char* name);
size_t getContentSize(const char* path);

bool isFolder(ContentData* data);
bool isEmptySubContent(char* subContentName);

#endif