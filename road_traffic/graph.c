
#include "graph.h"

#include <stdlib.h>

#include <stdio.h>

road* roads;//all roads
int length;//length of roads
int places;//number of cars
csgraph** locations;//locations in the graph

void graph_construction(graph** g, int length, int places){

    unsigned long int n;//number of nodes in the graph
    const int curpathmaxlength = 100;//maximal size of the current table of nodes
    int curpath[curpathmaxlength];//current path
    int index = 0, curpathindex;//respectively : indices in the table of all elements and the table of current nodes
    road r2;
    road curroads[curpathmaxlength];//current roads ->Must we add road or indices in graphs ?
    int ok, i;
    csgraph* ptrcsg;

    /* a lot of memory is allocated here */

    roads = road_generation(places, &n);
    locations = (csgraph**)calloc((int)n, sizeof(csgraph*));

    /* construction */

    //searches free index : if there is no free index, all roads have been used
    while(index!=-1){
        curpathindex = 0;//the beginning of the table of current roads
        curpath[0] = index;
        curroads[0] = roads[index];
        ok = 0;
        while(!ok){
            road_function(curroads[curpathindex], &r2);//the function we study is applied iteratively
            //r2 already visited in the current path?
            for(i=0;i<=curpathindex;i++){
                if(r2 == curroads[i]){
                    break;//we have found a circuit!
                }
            }
            if(i!=curpathindex+1){//there is a circuit
                //0 or 1 "normal" path (between 0 and i if i>0) + 1 circuit (between i and curpathindex)
                //here, we add a circuit and one path to the graph
                if(i>0){
                    ptrcsg = path_integration(g, curroads, i+1, PATH, NULL);
                    path_integration(g, curroads+i, curpathindex-i+1, CIRCUIT, ptrcsg);
                }else{
                    ptrcsg = path_integration(g, curroads, curpathindex+1, CIRCUIT, NULL);
                }
                //that block modifies the locations table
                for(i=0;i<=curpathindex;i++){
                    locations[curpath[i]] = ptrcsg;
                }
                break;//another path must be found
            }
            //r2 already visited before?
            index = road_search(0, n, r2);//printf("index : %d\n", index);
                curpathindex++;
                curpath[curpathindex] = index;//this table is updated
                curroads[curpathindex] = r2;//the current road is added to the table
            if(locations[index]){//the last node has been already visited
                //the path is added to its subgraph
                path_integration(g, curroads, curpathindex+1, PATH, locations[index]);
                //modifies the locations table
                for(i=0;i<curpathindex;i++){
                    locations[curpath[i]] = locations[index];
                }//remark : the last node is already visited so location has a correct value
                ok = 1;//STOP
            }

        }
        index = search_free_index((int)n);//we search the next node which will be at the beginning of the table of current roads
    }


    //the memory which has been allocated here is released
    free(locations);
    free(roads);
}

//that function finds a road which remains unconsidered so far
int search_free_index(int n){
    int i;
    for(i=0;i<n;i++){
        if(!locations[i]){
            return i;
        }
    }
    return -1;
}

//that function finds a given road in the table 'roads'
int road_search(int bg, int n, road r){
    int i;
    for(i=bg;i<n;i++){
        if(r == roads[i]){
            return i;
        }
    }
    for(i=0;i<bg;i++){
        if(r == roads[i]){
            break;
        }
    }
    return i;
}


//for adding an element to a graph
csgraph* path_integration(graph** g, road* curpath, int n, element elt, csgraph* csg){
    int i;

    if(!csg){
        //let's go the end of the list
        if(!(*g)){
            (*g) = (graph*)malloc(sizeof(graph));
        }else{
            while(*g){
                g = &((*g)->following);
            }
            (*g) = (graph*)malloc(sizeof(graph));
        }

        (*g)->following = NULL;
        (*g)->csg.elt = elt;
        (*g)->csg.following = NULL;

        switch(elt){
            case PATH:
            (*g)->csg.comp.cp.n = n;
            (*g)->csg.comp.cp.p = (road*)malloc(sizeof(road)*n);
            for(i=0;i<n;i++){
                (*g)->csg.comp.cp.p[i] = curpath[i];
            }
            break;
            default:
            (*g)->csg.comp.cc.n = n;
            (*g)->csg.comp.cc.c = (road*)malloc(sizeof(road)*n);
            for(i=0;i<n;i++){
                (*g)->csg.comp.cc.c[i] = curpath[i];
            }
        }
        return &((*g)->csg);
    }
    //otherwise, the component is added to the subgraph which is matched to the value of csg

    while(csg->following){
        csg = csg->following;
    }
    csg->following = (csgraph*)malloc(sizeof(csgraph));
    csg->following->following = NULL;
    csg->following->elt = elt;
    switch(elt){
    case PATH:
        csg->following->comp.cp.n = n;
        csg->following->comp.cp.p = (road*)malloc(sizeof(road)*n);
        for(i=0;i<n;i++){
            csg->following->comp.cp.p[i] = curpath[i];
        }
        break;
    default:
        csg->following->comp.cc.n = n;
        csg->following->comp.cc.c = (road*)malloc(sizeof(road)*n);
        for(i=0;i<n;i++){
            csg->following->comp.cc.c[i] = curpath[i];
        }
    }

    return NULL;
}

//number of connected subgraphs
int csgraphs_number(graph* g){
    int nb = 0;
    while(g){
        nb++;
        g = g->following;
    }
    return nb;
}

//this function writes a DOT file
void write_dotfile(char* filename, csgraph csg){
    FILE* f;
    csgraph* aux = &csg;
    int i;

    f = fopen(filename, "w");
    fprintf(f, "digraph G {\n\tnode [shape=circle, color=lightblue2, style=filled];\n");
    while(aux){
        fprintf(f, "\t");
        if(aux->elt==CIRCUIT){
            for(i=0;i<aux->comp.cc.n;i++){
                fprintf(f, "%u -> ", aux->comp.cc.c[i]);
            }
            fprintf(f, "%u;\n", aux->comp.cc.c[0]);
        }else{
            //path
            fprintf(f, "%u", aux->comp.cp.p[0]);
            for(i=1;i<aux->comp.cp.n;i++){
                fprintf(f, " -> %u", aux->comp.cp.p[i]);
            }
            fprintf(f, ";\n");
        }
        aux = aux->following;
    }
    fprintf(f, "}");
    fclose(f);
}

void graph_destruction(graph* g){
    graph* aux;

    while(g){
        aux = g->following;
        csgraph_destruction(&(g->csg));
        free(g);
        g = aux;
    }
}

void csgraph_destruction(csgraph* csg){
    //csg is a field of a variable of type graph
    csgraph* aux = csg->following;

    if(csg->elt==PATH){
        free(csg->comp.cp.p);
    }else{
        free(csg->comp.cc.c);
    }
    csg = aux;
    while(csg){
        aux = csg->following;
        if(csg->elt==PATH){
            free(csg->comp.cp.p);
        }else{
            free(csg->comp.cc.c);
        }
        free(csg);
        csg = aux;
    }
}

int circuit_length(csgraph* csg){
    while(csg->elt!=CIRCUIT){
        csg = csg->following;
    }
    return csg->comp.cc.n;
}

int nodes_number(csgraph* csg){
    int n = 0;
    while(csg){
        switch(csg->elt){
        case PATH:
            n += csg->comp.cp.n - 1;
            break;
        case CIRCUIT:
            n += csg->comp.cc.n;
            break;
        default:
            break;
        }
        csg = csg->following;
    }
    return n;
}

int tree_height(csgraph* csg){
    road stage[100];
    csgraph* ptr = csg;
    int i, j, n=1, h=0, add;

    while(ptr->elt!=CIRCUIT){
            ptr = ptr->following;
    }
    stage[0] = ptr->comp.cc.c[0];

    while(n!=0){
        h++;
        for(i=0;i<n;i++){
            ptr = csg;
            add = 0;
            while(ptr){
                if(ptr->elt==CIRCUIT){
                    ptr = ptr->following;
                    continue;
                }
                for(j=1;j<ptr->comp.cp.n;j++){
                    if(ptr->comp.cp.p[j]==stage[i]){
                        if(add){
                            stage[n] = ptr->comp.cp.p[j-1];
                            n++;
                        }else{
                            stage[i] = ptr->comp.cp.p[j-1];
                            add = 1;
                        }
                        break;
                    }
                }
                ptr = ptr->following;
            }
            if(!add){
                for(j=i;j<n;j++){
                    stage[j] = stage[j+1];
                }
                n--;
            }
        }
    }

    return h-1;
}
