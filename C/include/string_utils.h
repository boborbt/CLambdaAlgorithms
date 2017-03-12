#pragma once
#include "array.h"

// Splits the given string into pieces.
// Pieces are built such as String_join(String_split(string, delim), delim)
// equals the original string.
// Each piece is alloced via malloc and needs to be freed by the user.
Array* String_split(char*, char delim);


// Joins the givne string array into a single string
// joining a piece with the following by inserting the given
// delim char.
// The newbuilt string is alloced via malloc and needs to be
// freed by the user.
char* String_join(Array*, char delim);
