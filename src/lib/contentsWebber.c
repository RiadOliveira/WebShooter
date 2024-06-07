#include "dataGetter.h"
#include "libwst.h"

void handleContentsWebbing(WstParams*, ContentData*, char*);

void webContentsIntoArchive(WstParams* params, const char* archivePath) {
  ContentData currentContentData;
  char currentContentFullPath[PATH_MAX_SIZE];
  strcpy(currentContentFullPath, params->cwd);

  handleContentsWebbing(params, &currentContentData, currentContentFullPath);
}

void handleContentsWebbing(
  WstParams* params, ContentData* currentData, char* currentFullPath
) {
  const char** contentsPaths = params->contentOrArchivePaths;
  const size_t contentsQuantity = params->contentsOrArchivesQuantity;
  const size_t cwdLength = strlen(params->cwd);

  for(size_t ind = 0; ind < contentsQuantity; ind++) {
    const char* currentPath = contentsPaths[ind];

    getContentName(currentData->name, currentPath);
    appendPath(currentFullPath, cwdLength, currentPath);
    currentData->size = getContentSize(currentFullPath);
  }
}
