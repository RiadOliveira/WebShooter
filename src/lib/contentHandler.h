#ifndef CONTENT_HANDLER_H
#define CONTENT_HANDLER_H

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "errorThrower.h"
#include "fileManager.h"
#include "shortcutTypes.h"

typedef struct {
  char* name;
  Metadata metadata;
} ContentData;

void fillContentData(ContentData* data, const char* path);
void getContentName(char* name, const char* path);
void setContentMetadata(Metadata* metadata, const char* path);
void concatPathSeparatorToFolderName(char* name);

bool isFolder(ContentData* data);
bool isFile(ContentData* data);
bool isEmptySubContent(char* subContentName);

#endif