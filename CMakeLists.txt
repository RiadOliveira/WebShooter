cmake_minimum_required(VERSION 3.22)
project(WebShooter LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(MAIN_FILE "src/cli/cliwst.c")

set(HEADERS
  src/cli/cliTypes.h
  src/cli/cwdGetter.h
  src/cli/parser.h
  src/cli/validator.h

  src/lib/libTypes.h
  src/lib/libwst.h
  src/lib/pathHandler.h
  src/lib/typeChecker.h

  src/utils/errorThrower.h
  src/utils/mathUtils.h
  src/utils/shortcutTypes.h
)
set(SOURCES
  src/cli/cliwst.c
  src/cli/cwdGetter.c
  src/cli/parser.c
  src/cli/validator.c

  src/lib/libwst.c
  src/lib/pathHandler.c
  src/lib/typeChecker.c

  src/utils/errorThrower.c
  src/utils/mathUtils.c
)

list(REMOVE_ITEM SOURCES ${MAIN_FILE})

add_executable(wst ${MAIN_FILE} ${SOURCES})

target_include_directories(wst PRIVATE 
  src/lib
  src/utils
)