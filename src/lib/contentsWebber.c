#include "dataGetter.h"
#include "fileHandler.h"
#include "libwst.h"

void handleContentWebbing(FILE*, char*, ContentData*, const char*);

void webContentsIntoArchive(WstParams* params, const char* archivePath) {
  ContentData currentData;
  char buffer[BUFFER_SIZE];

  char currentFullPath[PATH_MAX_SIZE];
  strcpy(currentFullPath, params->cwd);

  const size_t cwdLength = strlen(params->cwd);
  appendPath(currentFullPath, cwdLength, archivePath);
  FILE* archive = openFileOrExit(currentFullPath, "wb");

  const char** contentsPaths = params->contentOrArchivePaths;
  const size_t contentsQuantity = params->contentsOrArchivesQuantity;

  for(size_t ind = 0; ind < contentsQuantity; ind++) {
    const char* currentPath = contentsPaths[ind];

    getContentName(currentData.name, currentPath);
    appendPath(currentFullPath, cwdLength, currentPath);
    currentData.size = getContentSize(currentFullPath);

    handleContentWebbing(archive, buffer, &currentData, currentFullPath);
  }

  fclose(archive);
}

void handleContentWebbing(
  FILE* archive, char* buffer, ContentData* data, const char* fullPath
) {
  const bool isFile = data->size != 0;

  if(isFile) {
    FILE* content = openFileOrExit(fullPath, "rb");

    size_t bytesRead;
    while((bytesRead = fread(buffer, 1, BUFFER_SIZE, content)) > 0) {
      fwrite(buffer, 1, bytesRead, archive);
    }

    fclose(content);
  }
}
