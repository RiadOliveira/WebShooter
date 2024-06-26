cmake_minimum_required(VERSION 3.22)
project(WebShooter LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(MAIN_FILE "src/cli/cliwst.c")

set(HEADERS
  src/cli/cliTypes.h
  src/cli/parser.h
  src/cli/validator.h

  src/lib/archiveUnwebber.h
  src/lib/bufferHandler.h
  src/lib/constants.h
  src/lib/contentHandler.h
  src/lib/contentsWebber.h
  src/lib/fileManager.h
  src/lib/libwst.h

  src/utils/errorThrower.h
  src/utils/mathUtils.h
  src/utils/shortcutTypes.h
)
set(SOURCES
  src/cli/cliwst.c
  src/cli/parser.c
  src/cli/validator.c

  src/lib/archiveUnwebber.c
  src/lib/bufferHandler.c
  src/lib/contentHandler.c
  src/lib/contentsWebber.c
  src/lib/fileManager.c

  src/utils/errorThrower.c
)

list(REMOVE_ITEM SOURCES ${MAIN_FILE})

add_executable(wst ${MAIN_FILE} ${SOURCES})

target_include_directories(wst PRIVATE 
  src/lib
  src/utils
)