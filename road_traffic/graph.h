#ifndef GRAPH_H
#define GRAPH_H

/** \file graph.h
 * \brief That file defines some structures and functions for graph building.
 * \details It enables us to build graphs related to the function applied to roads.
 * \author Alexandre MARIN
 *
 */

#include "road_traffic.h"

typedef enum element_ {PATH, CIRCUIT} element;

typedef struct path_{
    int n;
    road* p;
} path;

typedef struct circuit_ {
    int n;
    road* c;
} circuit;

typedef struct tree_ {
    int h;
    int* t;
} tree;

typedef union component_ {
    path cp;
    circuit cc;
} component;

typedef struct csgraph_ {
    component comp;
    element elt;
    struct csgraph_ * following;
} csgraph;//connected subgraph

typedef struct graph_ {
    csgraph csg;
    struct graph_ * following;
} graph;

/**
 * A global variable used during the building of a graph.
 */
extern road* roads;
extern int length;//not used for the time being
extern int places;
/**
 * A global variable used during the building of a graph.
 * Its enables us to save locations of nodes in the graph.
 */
extern csgraph** locations;

void graph_construction(graph** g, int length, int places);
int search_free_index(int n);
int road_search(int bg, int n, road r);

csgraph* path_integration(graph** g, road* curpath, int n, element elt, csgraph* csg);
int csgraphs_number(graph* g);
void write_dotfile(char* filename, csgraph csg);
//void btree_construction(graph* g);
//int btrees_number(csgraph* csg);
void graph_destruction(graph* g);
void csgraph_destruction(csgraph* csg);
int circuit_length(csgraph* csg);
int nodes_number(csgraph* csg);
int tree_height(csgraph* csg);


/** \typedef element
 * \details It refers to types of chains in graphs.
 */

/** \typedef path
 * \details It is a list of nodes. In the table, the n-th node is a predecessor of the (n+1)-th node.
 */

/** \typedef circuit
 * \details That structure is like path, but the last node is a predecessor of the first node in the table.
 */

/** \typedef csgraph
 * \details It represents connected subgraphs. A connected subgraphs is a list of components (circuits or paths).
 */

/** \typedef graph
 * \details It represents graphs. A graph is a list of connected subgraphs.
 */


/** \fn void graph_construction(graph** g, int length, int places)
 * \brief That function builds a piece of graph of the function we study.
 * \param length The length of roads
 * \param places The number of cars
 * \param g A graph
 * \return \c g A graph
 */

/** \fn csgraph* path_integration(graph** g, road* curpath, int n, element elt, csgraph* csg)
 * \brief That function adds an element to the graph \c g .
 * \param curpath The element to add
 * \param n The length of \c curpath
 * \param elt The type of element
 * \param g A graph
 * \param csg If its value is \c NULL , it means the element belongs to a new connected subgraph of \c g (that new subgraph will be created). Otherwise the element contains nodes which belong to the subgraph denoted by \c csg .
 * \return The updated graph \c g and the new subgraph created.
 */

/** \fn int csgraphs_number(graph* g)
 * \brief It counts the number of connected subgraphs in \c g .
 * \param g A graph
 * \return The number of connected subgraphs.
 */

/** \fn void write_dotfile(char* filename, csgraph csg)
 * \brief It writes a \c .dot file for saving a connected subgraph.
 * \param csg A connected subgraph
 * \param filename The name of the file which will be created. Be careful : if a file has the same name in the directory of the executable, it will be erased.
 */

/** \fn void graph_destruction(graph* g)
 * \brief It releases the memory allocated for \c g .
 * \param g A graph
 */

/** \fn int circuit_length(csgraph* csg)
 * \brief It computes the length of the circuit of the subgraph pointed to by \c csg .
 * \param csg A connected subgraph
 * \return The length of a circuit in a connected subgraph.
 */

/** \fn int nodes_number(csgraph* csg)
 * \brief It computes the number of nodes in the subgraph pointed to by \c csg .
 * \param csg A connected subgraph
 * \return The number of nodes of a connected subgraph.
 */

/** int tree_height(csgraph* csg)
 * \brief It computes the height of a tree in the subgraph pointed to by \c csg .
 * \details The algorithm builds all levels of a tree.
 * \param csg A connected subgraph
 * \return The height of a tree in a connected subgraph.
 */

#endif // GRAPH_H
