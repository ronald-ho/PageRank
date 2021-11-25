// This material is written with inspiration in course lectures
// Week 5 Graph ADT material

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"

Graph newGraph(int nV) {
    assert(nV >= 0);

    Graph graph = malloc(sizeof(*graph));
    assert(graph != NULL);

    graph->nV = nV;
    graph->nE = 0;
    // Allocate array of pointers out rows
    graph->edges = malloc(nV * sizeof(int *));
    assert(graph->edges != NULL);

    // Allocate memory for each column and initialise with 0
    for (int i = 0; i < nV; i++) {
        graph->edges[i] = calloc(nV, sizeof(int));
        assert(graph->edges[i] != NULL);
    }
    return graph;
}

void insertEdge(Graph graph, Vertex in, Vertex out, int weight) {
    assert(graph != NULL && validV(graph, in) && validV(graph, out));

    // As the weighted pagerank is directed only one vertex insert is needed
    // If the edges vertex inLink and outLink exists
    if (!graph->edges[in][out]) {
        graph->edges[in][out] = weight;
        graph->nE++;
    }
}

void removeEdge(Graph graph, Vertex in, Vertex out, int weight) {
    assert(graph != NULL && validV(graph, in) && validV(graph, out));

    // As the weighted pagerank is directed only one vertex remove is needed
    // If the edges vertex inLink and outLink exists
    if (graph->edges[in][out]) {
        graph->edges[in][out] = 0;
        graph->nE--;
    }
}

void freeGraph(Graph graph) {
    assert(graph != NULL);

    for (int i = 0; i < graph->nV; i++) {
        free(graph->edges[i]);
    }
    free(graph->edges);
    free(graph);
}


bool validV(Graph graph, Vertex v) {
    return (graph != NULL && v >= 0 && v < graph->nV);
}


bool adjacent(Graph graph, Vertex in, Vertex out) {
    assert(graph != NULL && validV(graph, in) && validV(graph, out));

    return (graph->edges[in][out] != 0);
}

int getnV(Graph graph) {
    return graph->nV;
}


// Function to calculate Win
double wInCount(Graph graph, Vertex v) {
    double inLink = 0;

    for (int in = 0; in < graph->nV; in++) {
        if (adjacent(graph, in, v)) {
            inLink++;
        }
    }

    return inLink;
}

// Function to find the sum of inLinks 
double sumInLinks(Graph graph, Vertex v) {
    double sum = 0;

    for (int in = 0; in < graph->nV; in++) {
        if (adjacent(graph, v, in)) {
            sum += wInCount(graph, in);
        }
    }

    return sum;
}


// Function to calculate Wout
double wOutCount(Graph graph, Vertex v) {
    double outLink = 0;

    for (int out = 0; out < graph->nV; out++) {
        if (adjacent(graph, v, out)) {
            outLink++;
        }
    }

    // If a node has zero outlinks, set the outlinks to be 0.5
    if (outLink == 0) {
        outLink = 0.5;
    }

    return outLink;
}



// Function to find the sum of outLinks
double sumOutLinks(Graph graph, Vertex v) {
    double sum = 0;

    for (int out = 0; out < graph->nV; out++) {
        if (adjacent(graph, v, out)) {
            sum += wOutCount(graph, out);
        }
    }

    return sum;
}