cmake_minimum_required(VERSION 3.22)
project(WebShooter LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(MAIN_FILE "src/cli/cliwst.c")

set(HEADERS
  src/cli/cliTypes.h
  src/cli/parsing.h
  src/cli/validation.h

  src/lib/libTypes.h
  src/lib/libwst.h

  src/utils/errorPrinting.h
  src/utils/mathUtils.h
  src/utils/shortcutTypes.h
)
set(SOURCES
  src/cli/cliwst.c
  src/cli/parsing.c
  src/cli/validation.c

  src/lib/libwst.c

  src/utils/errorPrinting.c
  src/utils/mathUtils.c
)

list(REMOVE_ITEM SOURCES ${MAIN_FILE})

add_executable(wst ${MAIN_FILE} ${SOURCES})

target_include_directories(wst PRIVATE 
  src/lib
  src/utils
)