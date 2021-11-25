#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>

// Graph representation is hidden
typedef struct GraphRep *Graph;

struct GraphRep {
    int **edges;    // Adjacency matrix
    int nV;         // Vertices
    int nE;         // Edge
};

// Vertices denoted by integers 0 to N - 1
typedef int Vertex;

// Edges are pairs of vertices (end-points)
typedef struct Edge { Vertex in; Vertex out;} Edge;

Graph newGraph(int nV);
void insertEdge(Graph graph, Vertex in, Vertex out, int weight);
void removeEdge(Graph graph, Vertex in, Vertex out, int weight);
void freeGraph(Graph graph);
bool adjacent(Graph graph, Vertex in, Vertex out);
int getnV(Graph graph);
double wInCount(Graph graph, Vertex v);
double wOutCount(Graph graph, Vertex v);
double sumInLinks(Graph graph, Vertex v);
double sumOutLinks(Graph graph, Vertex v);

// Check if a vertex is valid in a graph
bool validV(Graph graph, Vertex v);

#endif