#ifndef FILE_MANAGER
#define FILE_MANAGER

#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#include "constants.h"
#include "error.h"
#include "shortcutTypes.h"

#define DEFAULT_FOLDER_PERMISSIONS 0755

#define CWD "."
#define READ_BINARY_MODE "rb"
#define WRITE_BINARY_MODE "wb"

#define METADATA_SIZE sizeof(Metadata)
#define METADATA_MODE_SIZE sizeof(uint)
#define METADATA_SIZE_WITHOUT_SIZE_ATTRIBUTE (METADATA_SIZE - sizeof(size_t))

typedef struct {
  uint mode;
  uint uid, gid;
  long atime, mtime;
  size_t size;
} Metadata;

FILE* openFileOrExit(const char* path, const char* modes);
DIR* openFolderOrExit(const char* path);

void setFileOrFolderMetadata(const char* path, Metadata* metadata);
void createFolder(const char* path);
void appendPath(char* destination, unsigned int position, const char* source);

#endif