#ifndef __GENOME_H_
#define __GENOME_H_

#include <string.h>
#include <limits.h>

#define MAX_KMER_LEN 16

int getval(char c){
  switch (c){
    case 'A': case 'a':
      return 0;
    case 'C': case 'c':
      return 1;
    case 'G': case 'g':
      return 2;
    case 'T': case 't':
      return 3;
    default:
      return 0;
  }
  return 0;
}

typedef struct GNode{
  char key[MAX_KMER_LEN];/// Will hold strings only consisting of ACGT
  //int inEdges[4]; ///Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
  //int outEdges[4]; ///Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
  //struct GNode* inEdges[4]; ///Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
  //struct GNode* outEdges[4]; ///Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
  struct GNode* inEdges[5];
  int inCount;
  struct GNode* outEdges[5];
  int outCount;
  int key_len;/// this is our k length
}GNode;

void AddEdges(GNode* from, GNode* to){
      from->outEdges[getval(to->key[to->key_len-1])] = to;
      to->inEdges[getval(from->key[0])] = from;
}

int setGraphSize(int k_len){
      int GRAPH_SIZE = 1;
      switch(k_len){
          case 1: GRAPH_SIZE = 4; break;
          case 2: GRAPH_SIZE = 16; break;
          case 3: GRAPH_SIZE = 64; break;
          case 4: GRAPH_SIZE = 256; break;
          case 5: GRAPH_SIZE = 1024; break;
          case 6: GRAPH_SIZE = 4096; break;
          case 7: GRAPH_SIZE = 16384; break;
          case 8: GRAPH_SIZE = 65536; break;
          case 9: GRAPH_SIZE = 262144; break;
          case 10: GRAPH_SIZE = 1048576; break;
          case 11: GRAPH_SIZE = 4194304; break;
          default: GRAPH_SIZE = 16777216; break;
      }
      return GRAPH_SIZE;
}

void InitGNode(GNode* node, char* k, int kl){ // k = str, kl = k length
  int max_chars = kl < MAX_KMER_LEN ? kl : MAX_KMER_LEN;
  if(max_chars != kl){
    printf("Can't save the length of the key correctly. Exiting program.\n");
    exit(1);
  }
  strncpy(node->key, k, kl);
  node->key[kl] = '\0'; // End of character string
  node->key_len = kl;
  int i;
  for(i = 0; i < 5; i++)
  {
    node -> inEdges[i] = NULL;
    node -> outEdges[i] = NULL;
  }
  node -> inCount = 0;
  node -> outCount = 0;
}

/*long hash(GNode* node, long GRAPH_SIZE){///this can be used to determine if string exists already
  long toRet = 0; // 32 bits
  int i;
  for(i = 0; i < node->key_len; ++i){
    toRet = toRet << 2; // shift toRet bits left 2
    toRet |= getval(node->key[i]);
    printf("%ld\n", toRet);
  }
  toRet = toRet % GRAPH_SIZE;
  return toRet;
}*/

long hash(char* str, int key_len, long GRAPH_SIZE){///this can be used to determine if string exists already
  //printf("%s\n", str);
  long toRet = 0; // 32 bits
  int i;
  for(i = 0; i < key_len; ++i){
    toRet = toRet << 2; // shift toRet bits left 2
    toRet |= getval(str[i]);
    //printf("%ld\n", toRet);
  }
  toRet = toRet % GRAPH_SIZE;
  //printf("%ld\n", toRet);
  return toRet;
}

#endif //__GENOME_H_

