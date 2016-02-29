#include "dictionary.h"
#include "string.h"

int Dictionary_empty(Dictionary dictionary) {
  return Dictionary_size(dictionary) == 0;
}
