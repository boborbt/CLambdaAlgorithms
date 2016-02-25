#ifndef __KEY_INFO_H_JK3JNH9D873__
#define __KEY_INFO_H_JK3JNH9D873__

typedef struct _KeyInfo* KeyInfo;
typedef int (*KIComparator)(const void*, const void*);
typedef unsigned int (*KIHash)(const void*);

KeyInfo KeyInfo_new( KIComparator, KIHash );
void KeyInfo_delete(KeyInfo);
KIComparator KeyInfo_comparator(KeyInfo);
KIHash KeyInfo_hash(KeyInfo);

#endif
