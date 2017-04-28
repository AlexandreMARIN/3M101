
#include "road_traffic.hpp"
#include "graph.hpp"

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <windows.h>//suppress this line and the choice 0 if the program is not executed under Windows

int main(){

  int choice, length, i, j, csgn, lengthmax;
  road r, r2;
  graph* g, *ptr;
  char filename[50];

  printf("\nChoisissez une action :\n0 : animation\n1 : construction de graphes\n2 : construction de graphes simplifiés\n");
  scanf("%d", &choice);

  switch(choice){
  case 0 :
    printf("\nChoisissez une longueur de route : ");
    scanf("%d", &length);
    road::set_length(length);
    printf("\nChoisisez un entier pour l'initialisation : ");
    scanf("%d", &choice);
    r = choice;
    for(i=0;i<30;i++){
        road_function(r, &r2);
        road_show(r2);
        r=r2;
        Sleep(1000);
        system("cls");
    }
    return 0;

  case 1:
      printf("\nChoisissez une longueur de route minimale : ");
      scanf("%d", &length);
      printf("\nChoisissez une longueur de route maximale : ");
      scanf("%d", &lengthmax);
      for(road::set_length(length);road::len()<=lengthmax;road::set_length(road::len()+1)){
          for(i=1;i<=(road::len()>>1);i++){
            printf("\nnumber of cars : %d", i);
            g = NULL;
            graph_construction(&g, road::len(), i);
            printf("\n\tnumber of connected subgraphs : %d\n", csgn=csgraphs_number(g));

            circuits(g);//lengths of circuits

            //graphs are saved in files
            ptr = g;
            for(j=1;j<=csgn;j++){
                sprintf(filename, "graph_%d_%d_%d.gv", road::len(), i, j);
                write_dotfile(filename, ptr->csg);
                ptr = ptr->following;
            }

            nodes(g);//numbers of nodes

            trees_heights(g);//heights of trees

            graph_destruction(g);//it releases the memory

            group_subgraphs(road::len());//it creates an unique file for all components
          }
      }
    return 0;

    case 2:
      printf("\nChoisissez une longueur de route minimale : ");
      scanf("%d", &length);
      printf("\nChoisissez une longueur de route maximale : ");
      scanf("%d", &lengthmax);
      for(road::set_length(length);road::len()<=lengthmax;road::set_length(road::len()+1)){
          for(i=1;i<=(road::len()>>1);i++){
            printf("\nnumber of cars : %d", i);
            g = NULL;
            trees_construction(&g, road::len(), i);
            printf("\n\tnumber of connected subgraphs : %d\n", csgn=csgraphs_number(g));

            circuits(g);//lengths of circuits

            //graphs are saved in files
            ptr = g;
            for(j=1;j<=csgn;j++){
                sprintf(filename, "tree_%d_%d_%d.gv", road::len(), i, j);
                write_dotfile(filename, ptr->csg);
                ptr = ptr->following;
            }

            nodes(g);//numbers of nodes

            trees_heights(g);//heights of trees

            graph_destruction(g);//it releases the memory

          }
      }
    return 0;

  default:return 0;
  }


}

