#ifndef __KEY_INFO_H_JK3JNH9D873__
#define __KEY_INFO_H_JK3JNH9D873__

/*
 * KeyInfo maintains stores a pair of functions to compare and
 * create hashes of given data.
 */

typedef struct _KeyInfo* KeyInfo;
typedef int (*KIComparator)(const void*, const void*);
typedef unsigned int (*KIHash)(const void*);

/* Constructor and destructor*/
KeyInfo KeyInfo_new( KIComparator, KIHash );
void KeyInfo_free(KeyInfo);

/* Accessors */
KIComparator KeyInfo_comparator(KeyInfo);
KIHash KeyInfo_hash(KeyInfo);

#endif
