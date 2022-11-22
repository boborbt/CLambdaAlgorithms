#include <stdio.h>
#include <string.h>

#define NODE_STYLE "%d [label=\"(%s,%s)\"]\n"
#define OP_EDGE_STYLE(a) "%d -> %d [color=" a "]\n"
#define RET_EDGE_STYLE "%d -> %d [label = \"%ld\" style=dashed color=gray fontcolor=gray]\n"

// Min function
static long min(long x, long y) {
  if(x<y) {
    return x;
  } else {
    return y;
  }
}

static int new_freeid() {
  static int count = 1;

  return count++;
}

// Recursive function to compute the editing distance between string1 and string2.
static long editing_distance_dummy(const char* string1, const char* string2, int myid) {
  int insid = new_freeid();
  int delid = new_freeid();
  int nopid = new_freeid();

  if(string1[0] == '\0') {
    return (long) strlen(string2);
  }

  if(string2[0] == '\0') {
    return (long) strlen(string1);
  }

  printf(NODE_STYLE, delid, string1, string2+1);
  printf(OP_EDGE_STYLE("red"), myid, delid);
  long del_c = editing_distance_dummy(string1, string2+1, delid) + 1;
  printf(RET_EDGE_STYLE, delid, myid, del_c);


  printf(NODE_STYLE, insid, string1+1, string2);
  printf(OP_EDGE_STYLE("blue"), myid, insid);
  long copy_c = editing_distance_dummy(string1+1, string2, insid) + 1;
  printf(RET_EDGE_STYLE, insid, myid, copy_c);

  if(string1[0] == string2[0]) {
    printf(NODE_STYLE, nopid, string1+1, string2+1);
    printf(OP_EDGE_STYLE("gray"), myid, nopid);
    long result = min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1, nopid)));
    printf(RET_EDGE_STYLE, nopid, myid, result);

    return result;
  } else {
    return min(del_c, copy_c);
  }
}

int main()
{
    printf("digraph G {\n");
    printf("node [shape=Mrecord color=lightgray, style=filled]\n");
    printf("0 [label=\"(ciao,bao)\"]\n");
    editing_distance_dummy("ciao", "bao", 0);
    printf("\n}\n");
    return 0;
}
