cmake_minimum_required(VERSION 3.22)
project(WebShooter LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(MAIN_FILE "src/cli/main.c")

set(HEADERS
  src/cli/argumentsParsing.h
  src/cli/argumentsValidation.h

  src/utils/errorPrinting.h
  src/utils/shortcutTypes.h
)
set(SOURCES
  src/cli/argumentsParsing.c
  src/cli/argumentsValidation.c
  src/cli/main.c

  src/utils/errorPrinting.c
)

list(REMOVE_ITEM SOURCES ${MAIN_FILE})

add_executable(wst ${MAIN_FILE} ${SOURCES})

target_include_directories(wst PRIVATE 
  src/lib
  src/utils
)