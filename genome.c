#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genome.h"

#define BUFFSIZE 1023

void FreeHashGraph(GNode** hashGraph, int GRAPH_SIZE){
  ///loop through your hashGraph and free all existing nodes
  ///Then free the hashGraph
  int i;
  for(i = 0; i < GRAPH_SIZE; i++)
  {
    if(hashGraph[i] != NULL)
    {
      free(hashGraph[i]);
    }
  }
  free(hashGraph);
  return;
}


int InDegree(GNode* node)
{
  int i;
  int degree = 0;
  for(i = 0; i < 4; i++)
  {
    if(node -> inEdges[i] != NULL)
    {
      degree++;
    }
  }
  return degree;
}

int OutDegree(GNode* node)
{
  int i;
  int degree = 0;
  for(i = 0; i < 4; i++)
  {
    if(node -> outEdges[i] != NULL)
    {
      degree++;
    }
  }
  return degree;
}

int getLineLength(char* inputFilename)
{
  FILE* ifptr = fopen(inputFilename, "r");
  char buffer[BUFFSIZE];
  fgets(buffer, BUFFSIZE, ifptr);
  fseek(ifptr, 0, SEEK_SET);
  fclose(ifptr);
  return strlen(buffer) - 1;
}

void BuildGraph(char* inputFilename, char* nodeFilename, int k_len, GNode** hashGraph, int GRAPH_SIZE){
  int lineLen = getLineLength(inputFilename);
  FILE* ifptr = fopen(inputFilename, "r");
  if(!ifptr){
    printf("Could not open input file!!\n");
    FreeHashGraph(hashGraph, GRAPH_SIZE);
  }
  char* buffer = (char*)malloc(sizeof(char)*lineLen + 2); // Must be freed 
  GNode* prev=NULL, *curr=NULL;

  int count = 0;
  // Read each line
  while(fgets(buffer, lineLen + 2, ifptr))
  {
    count++;
    //printf("%d\n", count);
    prev = NULL;
    curr = NULL;
    int lower = 0;
    int upper = k_len - 1;
    while(upper < lineLen)
    {
      char* str = (char*)malloc(sizeof(char) * k_len + 1); //Reachability issue probably here
      int i;
      for(i = 0; i < k_len; i++)
      {
        str[i] = buffer[i + lower];
      }
      str[k_len] = '\0';


      int unique = 1;
      int hashIndex = hash(str, k_len, GRAPH_SIZE);
      if(hashGraph[hashIndex] != NULL)
      {
        unique = 0;
        curr = hashGraph[hashIndex];
      }
      
      if(unique == 1)
      {
        GNode* newNode = (GNode*)malloc(sizeof(GNode));
        InitGNode(newNode, str, k_len);
        hashGraph[hashIndex] = newNode;
        curr = newNode;
        //fprintf(uniqueOutput, "%s\n", curr -> key);
      }
      free(str);
      if(prev != NULL)
      {
        AddEdges(prev, curr);
        curr -> inCount++;
        prev -> outCount++;
      }

      prev = curr;
      upper++;
      lower++;
    }
  }
  fclose(ifptr);
  free(buffer);
}

void PrintUniqueNodes(char* nodeFilename, GNode** hashGraph, int GRAPH_SIZE){
  FILE* nodeOut = fopen(nodeFilename, "w");
  //do some sort of loop to print all unique nodes here
  int i = 0;
  for(i = 0; i < GRAPH_SIZE; i++)
  {
    if(hashGraph[i] != NULL)
    {
      fprintf(nodeOut, "%s\n", hashGraph[i] -> key);
    }
  }
  fclose(nodeOut);
}


void TraverseHelper(GNode* node, FILE* outfile){
  fprintf(outfile, "%c", node->key[node->key_len-1]);
  if(InDegree(node) == 1 && OutDegree(node) == 1){
    for(int j = 0; j < 4; ++j){
      if (node->outEdges[j]){
        TraverseHelper(node->outEdges[j], outfile);
        break;
      }
    }
  }
}

void TraverseGraph(GNode** hashGraph, int k_len, int GRAPH_SIZE, char* outputFilename){
  FILE* outfile = fopen(outputFilename, "w");
  //Do some sort of loop that will start a traversal at every hnode
  // use TraverseHelper to traverse after we are at hnode to make this easier
  int i;
  int j;
  for(i = 0; i < GRAPH_SIZE; i++)
  {
    GNode* node = hashGraph[i];
    if(node != NULL && !(InDegree(node) == 1 && OutDegree(node) == 1))
    {
      for(j = 0; j < 4; j++)
      {
        if(node -> outEdges[j] != NULL)
        {
          fprintf(outfile, "%s", hashGraph[i] -> key);
          TraverseHelper(node -> outEdges[j], outfile);
          fprintf(outfile, "\n");
        }
      }
    }
  }
  fclose(outfile);
}


int main(int argc, char* argv[]){
  // argv[1] should be an integer of what our k length should be.
  // argv[2] will be the input filename that will contain the different reads. Each line in the
  // input file corresponds to one read.
  // argv[3] will be the filename of where you should output the strings from your traversal
  
  /*if(argc != 5){
    printf("Usage: ./pa3 <k_len> <inputFilename> <outputFilename> <nodeoutputFilename>\n");
    exit(1);
  }*/

  if(argc != 4 && argc != 5)
  {
    printf("Wrong arguments\n");
    return EXIT_FAILURE;
  }

  int k_len = atoi(argv[1]); // convert string to number (str) -> (int)
  if(k_len < 1){
    printf("k_len must be an integer greater than 0\n");
    exit(1);
  }

  char* inputFilename = argv[2];
  char* outputFilename = argv[3];

  int GRAPH_SIZE = setGraphSize(k_len);///defined in genome.h
  GNode** hashGraph = (GNode**)malloc(sizeof(GNode*)* GRAPH_SIZE);
  int i;
  for(i = 0; i < GRAPH_SIZE; i++)
  {
    hashGraph[i] = NULL;//intialize just in case
  }

  ///Build the graph from the input file
  BuildGraph(inputFilename, outputFilename, k_len, hashGraph, GRAPH_SIZE);

  if(argc == 5)
  {
    char* nodeFilename = argv[4];
    PrintUniqueNodes(nodeFilename, hashGraph, GRAPH_SIZE);
    FreeHashGraph(hashGraph, GRAPH_SIZE);
    return 0;
  }

  TraverseGraph(hashGraph, k_len, GRAPH_SIZE, outputFilename);


  ///Make sure we free the hashGraph correctly.
  FreeHashGraph(hashGraph, GRAPH_SIZE);

  return 0;
}





