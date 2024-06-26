#ifndef CONTENT_HANDLER_H
#define CONTENT_HANDLER_H

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "errorThrower.h"
#include "fileManager.h"
#include "shortcutTypes.h"

#define METADATA_SIZE sizeof(Metadata)
#define METADATA_SIZE_WITHOUT_SIZE_ATTRIBUTE (METADATA_SIZE - sizeof(size_t))

typedef struct {
  char* name;
  Metadata metadata;
} ContentData;

void fillContentData(ContentData* data, const char* path);
void getContentName(char* name, const char* path);
void setContentMetadata(Metadata* metadata, const char* path);

size_t getMetadataStructSize(Metadata* metadata);
bool isFile(Metadata* metadata);
bool isFolder(Metadata* metadata);
bool isEmptySubContent(char* subContentName);

#endif