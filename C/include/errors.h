#pragma once

typedef struct _Error Error;

typedef enum {
  ERROR_GENERIC = 100,
  ERROR_ARGUMENT_PARSING,
  ERROR_FILE_OPENING,
  ERROR_FILE_READING,
  ERROR_FILE_WRITING,
  ERROR_FILE_LOCKING,
  ERROR_INDEX_OUT_OF_BOUND,
  ERROR_ITERATOR_MISUSE
} ErrorCode;

// Creates a new error with the given code and error message
Error* Error_new(ErrorCode code, const char* formatted_message, ...);

// Frees memory alloced by Error_new
void Error_free(Error*);

// Raises the error. It prints the error message on stderr and exits with
// the error code provided by "error".
__attribute__((noreturn)) void Error_raise(Error* error);

// Raises the error and exists silently. Just like Error_raise, but prints
// nothing on stderr.
__attribute__((noreturn)) void Error_raise_silent(Error* error);

// Given an error returns the contained error message.
const char* Error_message(Error* error);

// Given an error returns the contained error code.
ErrorCode Error_error_code(Error* error);

// Returns a string representation of the error code.
const char* Error_code_to_string(ErrorCode code);

// Prints an error to stderr (it prints both the error code and the error message).
Error* Error_print(Error* error);
