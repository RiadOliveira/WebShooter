#ifndef CONTENT_HANDLER
#define CONTENT_HANDLER

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "error.h"
#include "fileManager.h"
#include "shortcutTypes.h"

#define FOLDER_TERMINATOR PATH_SEPARATOR

typedef struct {
  char name[CONTENT_NAME_MAX_SIZE];
  Metadata metadata;
} ContentData;

void getContentData(ContentData* data, const char* path);
void getContentName(char* name, const char* path);
void getContentMetadata(Metadata* metadata, const char* path);

size_t getMetadataStructSize(Metadata* metadata);
bool isFile(Metadata* metadata);
bool isFolder(Metadata* metadata);
bool isEmptySubContent(char* subContentName);

#endif