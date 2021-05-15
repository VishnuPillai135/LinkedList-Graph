#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>


// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// graph representation is hidden
typedef struct GraphRep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int validV (Graph, Vertex); // validity check


// edges are pairs of vertices (end-points)
typedef struct {
	Vertex v;
	Vertex w;
} Edge;

void insertEdge (Graph, Vertex, Vertex, int);
void removeEdge (Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph (int nV);
Graph makeGraph (int, int **);
void printGraph (Graph, int);

#endif