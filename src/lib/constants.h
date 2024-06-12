#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PATH_MAX_SIZE 4096
#define CONTENT_NAME_MAX_SIZE 255

#define BUFFER_MAX_SIZE 1024 * 1000
#define BUFFERS_QUANTITY 2

#define NULL_TERMINATOR "\0"

#if defined(_WIN32) || defined(_WIN64)
  #define PATH_SEPARATOR "\\"
#else
  #define PATH_SEPARATOR "/"
#endif

#endif