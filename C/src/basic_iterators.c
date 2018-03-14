#include "basic_iterators.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mem.h"
#include "errors.h"


// --------------------------------------------------------------------------------
// NUMBER ITERATOR
// --------------------------------------------------------------------------------

typedef struct {
  unsigned long* end;
  unsigned long current;
} NumberIterator;

static NumberIterator* NumberIterator_new(void* container) {
  NumberIterator* result = (NumberIterator*) Mem_alloc(sizeof(NumberIterator));
  result->end =  (unsigned long*) container;
  result->current = 0;

  return result;
}


static unsigned long* Number_new(unsigned long n) {
  unsigned long* it = (unsigned long*) Mem_alloc(sizeof(unsigned long));
  *it = n;

  return it;
}

static void NumberIterator_next(NumberIterator* iterator) {
  iterator->current++;
}

static void* NumberIterator_get(NumberIterator* iterator) {
  return &iterator->current;
}

static long NumberIterator_end(NumberIterator* iterator) {
  return iterator->current >= *iterator->end;
}

static long NumberIterator_same(NumberIterator* lhs, NumberIterator* rhs) {
  return *lhs->end == *rhs->end && lhs->current == rhs->current;
}

static void NumberIterator_free(NumberIterator* iterator) {
  Mem_free(iterator->end);
  Mem_free(iterator);
}

Iterator Number_it(unsigned long n) {
  Iterator result;

  result.container = Number_new(n);
  result.new_iterator = (void* (*)(void*)) NumberIterator_new;
  result.next = (void (*)(void*)) NumberIterator_next;
  result.get = (void* (*)(void*)) NumberIterator_get;
  result.end = (int   (*)(void*)) NumberIterator_end;
  result.same =(int   (*)(void*, void*)) NumberIterator_same;
  result.free =(void  (*)(void*)) NumberIterator_free;

  return result;
}


// --------------------------------------------------------------------------------
// TextFile Iterator
// --------------------------------------------------------------------------------

typedef struct {
  char* filename;
  char delimiter;
} TFIFileInfo;  // Text File Iterator file info

typedef struct {
  TFIFileInfo* file_info;
  FILE* file;
  char* buf;
  size_t buf_len;
} TextFileIterator;


static TFIFileInfo* TFIFileInfo_new(const char* filename, char delimiter) {
  TFIFileInfo* result = (TFIFileInfo*) Mem_alloc(sizeof(TFIFileInfo));
  result->filename = (void*) Mem_strdup(filename);
  result->delimiter = delimiter;

  return result;
}

static void TextFileIterator_next(TextFileIterator* iterator) {
  int end_of_file = feof(iterator->file);

  if(end_of_file && iterator->buf != NULL) {
    free(iterator->buf);
    iterator->buf = NULL;
    return;
  }

  errno = 0;
  long nchars_read = getdelim(&iterator->buf, &iterator->buf_len, iterator->file_info->delimiter, iterator->file);
  end_of_file = feof(iterator->file);

  if(nchars_read < 0 && !end_of_file) {
      Error_raise(Error_new(ERROR_FILE_READING, "Error reading input file (reason:%s)", strerror(errno)));
  }

  if(nchars_read <= 0 && end_of_file) {
    free(iterator->buf);
    iterator->buf = NULL;
    return;
  }

  if(iterator->buf[nchars_read-1] == iterator->file_info->delimiter) {
    iterator->buf[nchars_read-1] = '\0';
  }
}

static TextFileIterator* TextFileIterator_new(TFIFileInfo* file_info) {
  TextFileIterator* result = (TextFileIterator*) Mem_alloc(sizeof(TextFileIterator));

  result->file_info = file_info;

  result->file = fopen(file_info->filename, "r");
  if(result->file == NULL) {
    Error_raise(Error_new(ERROR_FILE_OPENING,
      "Error opening file %s, reason: %s", file_info->filename, strerror(errno)));
  }

  result->buf = NULL;
  result->buf_len = 0;

  TextFileIterator_next(result);

  return result;
}

static void* TextFileIterator_get(TextFileIterator* iterator) {
  return iterator->buf;
}

static int TextFileIterator_end(TextFileIterator* iterator) {
  return iterator->buf == NULL;
}

static int TextFileIterator_same(TextFileIterator* it1, TextFileIterator* it2) {
  return !strcmp(it1->file_info->filename, it2->file_info->filename) && ftell(it1->file) == ftell(it2->file);
}

static void TextFileIterator_free(TextFileIterator* iterator) {
  if(iterator->buf!=NULL) {
    // This should *not* be freed using Mem_free since it is
    // alloced by getline using the standard 'malloc' function.
    free(iterator->buf);
  }

  fclose(iterator->file);

  Mem_free(iterator->file_info->filename);
  Mem_free(iterator->file_info);
  Mem_free(iterator);
}


Iterator TextFile_it(const char* filename, char delimiter) {
  return Iterator_make(
    TFIFileInfo_new(filename, delimiter),
    (void* (*)(void*)) TextFileIterator_new,
    (void (*)(void*)) TextFileIterator_next,
    (void* (*)(void*)) TextFileIterator_get,
    (int   (*)(void*)) TextFileIterator_end,
    (int   (*)(void*, void*)) TextFileIterator_same,
    (void  (*)(void*)) TextFileIterator_free
  );
}
