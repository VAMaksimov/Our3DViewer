#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#include <cstdio>

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ESCAPE 27
#define ENTER_KEY 10
#define SPACE_KEY 32

// extern is for include/interface.h
extern const float TIMEOUTS[10];

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef enum {
  ERROR_CODE_SUCCESS = 0,
  ERROR_CODE_FILE_NOT_FOUND,
  ERROR_CODE_INVALID_FORMAT,
  ERROR_CODE_MEMORY_ALLOCATION_FAILED,
  ERROR_CODE_UNSUPPORTED_VERSION,
  ERROR_CODE_UNKNOWN_ERROR
} ErrorCode;

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#endif  // INCLUDE_COMMON_H