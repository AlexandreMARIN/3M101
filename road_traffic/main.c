
#include "road_traffic.h"
#include "graph.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){

/*  road r=3u, r2, r3;
  road* ptr;
  int i;
  for(i=0;i<1000;i++){
    road_function(r, &r2);
    road_show(r2);
    r3=r;
    r=r2;
    r2=r3;
    usleep(20000);
    system("cls");
  }
*/

  int i, j;
  int csgn;
  graph* g, *ptr;
  char filename[50];

  for(i=0;i<=(BITSIZE>>1);i++){
    printf("\nnumber of cars : %d", i);
    g = NULL;
    graph_construction(&g, BITSIZE, i);
    printf("\n\tnumber of connected subgraphs : %d\n", csgn=csgraphs_number(g));
    ptr = g;
    printf("\n\tcircuits lengths :");
    for(j=1;j<=csgn;j++){
        printf("\n\t\t%d", circuit_length(&(ptr->csg)));
        //sprintf(filename, "graph_%d_%d_%d.dot", BITSIZE, i, j);
        //write_dotfile(filename, ptr->csg);
        ptr = ptr->following;
    }
    ptr = g;
    printf("\n\tnodes numbers :");
    for(j=1;j<=csgn;j++){
        printf("\n\t\t%d", nodes_number(&(ptr->csg)));
        ptr = ptr->following;
    }
    ptr = g;
    printf("\n\ttrees heights :");
    for(j=1;j<=csgn;j++){
        printf("\n\t\t%d", tree_height(&(ptr->csg)));
        ptr = ptr->following;
    }
    graph_destruction(g);
  }


  return 0;
}
