#pragma once

typedef struct _Error* Error;

typedef enum {
  ERROR_GENERIC = 100,
  ERROR_ARGUMENT_PARSING,
  ERROR_FILE_OPENING,
  ERROR_FILE_READING,
  ERROR_FILE_WRITING,
  ERROR_FILE_LOCKING,
  ERROR_INDEX_OUT_OF_BOUND
} ErrorCode;

Error Error_new(ErrorCode code, const char* formatted_message, ...);
__attribute__((noreturn)) void Error_raise(Error error);
__attribute__((noreturn)) void Error_raise_silent(Error error);

const char* Error_message(Error error);
ErrorCode Error_error_code(Error error);
const char* Error_code_to_string(ErrorCode code);
Error Error_print(Error error);
