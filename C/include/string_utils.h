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

/**
 * Fill buf with n repetitions of character c
 * 
 * # parameters
 * 
 * - buf: the buffer where to return the result. It should hold enough space 
 *        to store n-1 characters + '\0'
 * - c: the character to repeat
 * - n: number of repetitions
 * 
 * # results
 * 
 * String_repeatchar(str, '-', 10) would fill str with the string "----------"
 */
char* String_repeatchar(char* str, char c, unsigned long n);

/**
 * Fill buf with a line of at most n repetitions of character c. The actual
 * number of repetitions depends on TL the line length of the current terminal.
 * The actual number of repetitions will be min(TL, n)
 *
 * # parameters
 *
 * - buf: the buffer where to return the result. It should hold enough space
 *          to store n-1 characters + '\0'
 * - c: the character to repeat
 * - n: maximal number of repetitions
 */
char* String_line_with_termsize(char* buf, char c, unsigned long n);

/**
 * Constructs and returns a string containing the given label surrounded by
 * square brackets and centered in a field of length equal to the current
 * terminal line size (or BUF_SIZE if the line is larger than BUF_SIZE).
 * At each side of the label characters are filled with the given c symbol.
 * ANSI color characters are also inserted so to make everything bold and white
 * with the exception of the label which is rendered in bold and green.
 *
 * # parameters:
 *
 * - hl: a character array with enough space to hold at least n characters
 * - label: the label to be rendered
 * - c: the character to be repeated at each side of the label
 * - n: size of the hl buffer.
 *
 * # result
 *
 * head_line(hl, "test", '=', 100) and assuming a terminal line length of 22
 * would copy the string "========[test]========" into the hl buffer.
 */
char* String_head_line(char* hl, char* label, char c, unsigned long n);
