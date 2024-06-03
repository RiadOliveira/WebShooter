cmake_minimum_required(VERSION 3.22)
project(WebShooter LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(MAIN_FILE "src/cli/main.c")

set(HEADERS
  
)
set(SOURCES
  src/cli/main.c
)

list(REMOVE_ITEM SOURCES ${MAIN_FILE})

add_executable(webShooter ${MAIN_FILE} ${SOURCES})

target_include_directories(webShooter PRIVATE 
  src/cli
  src/lib
)