#include "basic_iterators.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mem.h"
#include "errors.h"
#include "macros.h"


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
  return Iterator_make(
    Number_new(n),
    (void* (*)(void*))        NumberIterator_new,
    (void  (*)(void*))        NumberIterator_next,
    (void* (*)(void*))        NumberIterator_get,
    (int   (*)(void*))        NumberIterator_end,
    (int   (*)(void*, void*)) NumberIterator_same,
    (void  (*)(void*))        NumberIterator_free
  );
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

  if(end_of_file) {
    if(iterator->buf != NULL) {
      free(iterator->buf);
      iterator->buf = NULL;
    }
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
    (void* (*)(void*))        TextFileIterator_new,
    (void  (*)(void*))        TextFileIterator_next,
    (void* (*)(void*))        TextFileIterator_get,
    (int   (*)(void*))        TextFileIterator_end,
    (int   (*)(void*, void*)) TextFileIterator_same,
    (void  (*)(void*))        TextFileIterator_free
  );
}

// --------------------------------------------------------------------------------
// Char iterator
// --------------------------------------------------------------------------------

typedef struct {
  char* string;
  size_t position;
  size_t end;
} CharIterator;

static void* CharIterator_new(char* string) {
  CharIterator* iterator = (CharIterator*) Mem_alloc(sizeof(CharIterator));
  iterator->string = string;
  iterator->position = 0;
  iterator->end = strlen(string) - 1;

  return iterator;
}

static void CharIterator_next(CharIterator* iterator) {
  ++iterator->position;
}

static char* CharIterator_get(CharIterator* iterator) {
  return &iterator->string[iterator->position];
}

static int CharIterator_end(CharIterator* iterator) {
  return iterator->position != (size_t) -1 && iterator->position > iterator->end;
}

static int CharIterator_same(CharIterator* it1, CharIterator* it2) {
  return it1->string == it2->string && it1->position == it2->position;
}

static void CharIterator_free(CharIterator* iterator) {
  Mem_free(iterator);
}

static void CharIterator_prev(CharIterator* iterator) {
  --iterator->position;
}

static void CharIterator_to_begin(CharIterator* iterator) {
  iterator->position = 0;
}

static void CharIterator_to_end(CharIterator *iterator) {
  iterator->position = iterator->end;
}

static void CharIterator_move_to(CharIterator* iterator, size_t new_pos) {
  iterator->position = new_pos;
}

static size_t CharIterator_size(CharIterator* iterator) {
  return iterator->end + 1;
}

static void CharIterator_set(CharIterator* iterator, char* ch) {
  iterator->string[iterator->position] = *ch;
}

static void* CharIterator_alloc_obj(CharIterator* UNUSED(iterator)) {
  return  Mem_alloc(sizeof(char));
}

static void* CharIterator_copy_obj(CharIterator* iterator, void* to_mem) {
  char* to_mem_ch = (char*) to_mem;
  *to_mem_ch = *CharIterator_get(iterator);

  return to_mem;
}

static void CharIterator_free_obj(void* obj) {
  Mem_free(obj);
}

Iterator Char_it(char* string) {
  Iterator result = Iterator_make(
    string,
    (void* (*)(void*))        CharIterator_new,
    (void  (*)(void*))        CharIterator_next,
    (void* (*)(void*))        CharIterator_get,
    (int   (*)(void*))        CharIterator_end,
    (int   (*)(void*, void*)) CharIterator_same,
    (void  (*)(void*))        CharIterator_free
  );

  result = BidirectionalIterator_make(
    result,
    (void  (*)(void*)) CharIterator_prev,
    (void  (*)(void*)) CharIterator_to_begin,
    (void  (*)(void*)) CharIterator_to_end
  );

  result = MutableIterator_make(
    result,
    (void  (*)(void*, void*)) CharIterator_set
  );

  result = RandomAccessIterator_make(
    result,
    (void (*)(void*, size_t)) CharIterator_move_to,
    (size_t (*)(void*)) CharIterator_size
  );

  result = CloningIterator_make(
    result,
    (void* (*)(void*)) CharIterator_alloc_obj,
    (void* (*)(void*, void*)) CharIterator_copy_obj,
    (void (*)(void*)) CharIterator_free_obj
  );

  return result;
}



// --------------------------------------------------------------------------------
// CArray Iterator
// --------------------------------------------------------------------------------

typedef struct {
  unsigned char* carray;
  size_t count;
  size_t width;
  size_t ref_count;
} CArrayInfo;

typedef struct  {
  CArrayInfo* info;
  size_t position;
} CArrayIterator;

static CArrayInfo* CArrayInfo_new(void* carray, size_t count, size_t width) {
  CArrayInfo* result = (CArrayInfo*) Mem_alloc(sizeof(CArrayInfo));
  result->carray = (unsigned char*) carray;
  result->count = count;
  result->width = width;
  result->ref_count = 0;
  return result;
}

static void* cit_pos(CArrayIterator* iterator) {
  return iterator->info->carray + iterator->position * iterator->info->width;
}


static CArrayIterator* CArrayIterator_new(CArrayInfo* info) {
  info->ref_count += 1;

  CArrayIterator* result = (CArrayIterator*) Mem_alloc(sizeof(CArrayIterator));
  result->info = info;
  result->position = 0;

  return result;
}

static void CArrayIterator_next(CArrayIterator* iterator) {
  iterator->position += 1;
}

static void* CArrayIterator_get(CArrayIterator* iterator) {
  return cit_pos(iterator);
}

static void CArrayIterator_move_to(CArrayIterator* iterator, size_t new_pos) {
  iterator->position = new_pos;
}

static size_t CArrayIterator_size(CArrayIterator* iterator) {
  return iterator->info->count;
}

static int CArrayIterator_end(CArrayIterator* iterator) {
  return iterator->position == (size_t) -1 || iterator->position >= iterator->info->count;
}

static int CArrayIterator_same(CArrayIterator* it1, CArrayIterator* it2) {
  return it1->info == it2->info &&
    memcmp(cit_pos(it1), cit_pos(it2), it1->info->width) == 0 &&
    it1->position == it2->position;
}

static void CArrayIterator_free(CArrayIterator* iterator) {
  iterator->info->ref_count -= 1;

  if(iterator->info->ref_count == 0) {
    Mem_free(iterator->info);
  }

  Mem_free(iterator);
}

static void CArrayIterator_prev(CArrayIterator* iterator) {
  iterator->position -= 1;
}

static void CArrayIterator_to_begin(CArrayIterator* iterator) {
  iterator->position = 0;
}

static void CArrayIterator_to_end(CArrayIterator *iterator) {
  iterator->position = iterator->info->count - 1;
}

static void CArrayIterator_set(CArrayIterator* iterator, void* obj) {
  memcpy( cit_pos(iterator), obj, iterator->info->width );
}

static void* CArrayIterator_alloc_obj(CArrayIterator* iterator) {
  return Mem_alloc(iterator->info->width);
}

static void* CArrayIterator_copy_obj(CArrayIterator* iterator, void* to_mem) {
  memcpy( to_mem, cit_pos(iterator), iterator->info->width );
  return to_mem;
}

static void CArrayIterator_free_obj(void* obj)  {
  Mem_free(obj);
}

Iterator CArray_it(void* carray, size_t count, size_t width) {
  Iterator result = Iterator_make(
    CArrayInfo_new(carray, count, width),
    (void* (*)(void*))        CArrayIterator_new,
    (void  (*)(void*))        CArrayIterator_next,
    (void* (*)(void*))        CArrayIterator_get,
    (int   (*)(void*))        CArrayIterator_end,
    (int   (*)(void*, void*)) CArrayIterator_same,
    (void  (*)(void*))        CArrayIterator_free
  );

  result = BidirectionalIterator_make(
    result,
    (void  (*)(void*)) CArrayIterator_prev,
    (void  (*)(void*)) CArrayIterator_to_begin,
    (void  (*)(void*)) CArrayIterator_to_end
  );

  result = RandomAccessIterator_make(
    result,
    (void (*)(void*, size_t)) CArrayIterator_move_to,
    (size_t (*)(void*)) CArrayIterator_size
  );

  result = MutableIterator_make(
    result,
    (void  (*)(void*, void*)) CArrayIterator_set
  );

  result = CloningIterator_make(
    result,
    (void* (*)(void*)) CArrayIterator_alloc_obj,
    (void* (*)(void*, void*)) CArrayIterator_copy_obj,
    (void (*)(void*)) CArrayIterator_free_obj
  );

  return result;
}
