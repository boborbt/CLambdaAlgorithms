#include "errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "mem.h"

struct _Error {
  char* message;
  ErrorCode code;
};


const char* Error_code_to_string(ErrorCode code) {
  switch(code) {
    case ERROR_GENERIC:
      return "INTERNAL ERROR";
    case ERROR_FILE_READING:
    case ERROR_FILE_WRITING:
    case ERROR_FILE_OPENING:
    case ERROR_FILE_LOCKING:
      return "FILE ERROR";
    case ERROR_INDEX_OUT_OF_BOUND:
      return "INDEX OUT OF BOUND ERROR";
    case ERROR_ARGUMENT_PARSING:
      return "PARSING ARGUMENT ERROR";
    case ERROR_ITERATOR_MISUSE:
      return "ITERATOR MISUSE ERROR";
  }
}

Error* Error_print(Error* error) {
  fprintf(stderr, "%s: %s\n", Error_code_to_string(error->code), error->message);
  return error;
}

__attribute__((noreturn)) void Error_raise_silent(Error* error) {
  exit(error->code);
}

__attribute__((noreturn)) void Error_raise(Error* error)  {
  Error_print(error);
  Error_raise_silent(error);
}

Error* Error_new(ErrorCode code, const char* formatted_message, ...) {
  Error* error = (Error*) Mem_alloc(sizeof(struct _Error));
  error->code = code;

  va_list args;
  va_start(args, formatted_message);
  vasprintf(&error->message, formatted_message, args);
  va_end(args);

  return error;
}

void Error_free(Error* error)  {
  Mem_free(error);
}

ErrorCode Error_error_code(Error* error) {
  return error->code;
}

const char* Error_message(Error* error) {
  return error->message;
}
