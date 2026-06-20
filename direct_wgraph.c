//
// Created by Spara on 20/06/2026.
//

#include "direct_wgraph.h"

#include <stddef.h>
#include <stdlib.h>


#define DEFAULT_NODES_CAPACITY 10
#define DEFAULT_EDGES_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct strWGNode {
    void* val;
}DWGraph_node;

typedef struct strEdge {
    DWGraph_node* from;
    DWGraph_node* to;
    void* weight;
}DWGraph_edge;

struct strDWGraph {
    DWGraph_node* nodes;
    DWGraph_edge* edges;
    size_t numNodes; //num of active nodes
    size_t numEdges; //num of active edges
    size_t nodesCapacity;
    size_t edgesCapacity;
};



DWGraph dwgraph_create(void) {
    DWGraph graph = malloc(sizeof( struct strDWGraph));
    if (graph == NULL) return graph;

    graph->nodes = calloc(DEFAULT_NODES_CAPACITY,sizeof(DWGraph_node));
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

    graph->edges = calloc(DEFAULT_EDGES_CAPACITY,sizeof(DWGraph_edge));
    if (graph->edges == NULL) {
        free(graph->nodes);
        free(graph);
        return NULL;
    }

    graph->numEdges = 0;
    graph->numNodes = 0;
    graph->nodesCapacity = DEFAULT_NODES_CAPACITY;
    graph->edgesCapacity = DEFAULT_EDGES_CAPACITY;

    return graph;
}



DWGraph_statusCodes dwgraph_destroy(DWGraph* graph) {
    //tests di robustezza
    if (graph == NULL || *graph == NULL ) return DWGraph_ERROR_NULL_POINTER;
    if ((*graph)->edges == NULL || (*graph)->nodes == NULL) return DWGraph_ERROR_SEMANTIC_FAILURE;

    free((*graph)->edges);
    free((*graph)->nodes);

    free(*graph);
    *graph = NULL;

    return DWGraph_OK;
}


DWGraph_statusCodes dwgraph_add_node(DWGraph graph, void* node) {

    //tests di robustezza
    if (graph == NULL || node == NULL) return DWGraph_ERROR_NULL_POINTER;

    if (graph->numNodes == (graph->nodesCapacity)-1) {

        size_t newSize = graph->nodesCapacity * GROWTH_FACTOR;
        if (newSize <= graph->nodesCapacity) return DWGraph_ERROR_ARITHMETIC_OVERFLOW;

        DWGraph_node* temp = realloc(graph->nodes, newSize);
        if (temp == NULL) return DWGraph_ERROR_ALLOCATION_FAILURE;

        graph->nodes = temp;
    }

    graph->nodes[graph->numNodes].val = node;
    graph->numNodes +=1;

    return DWGraph_OK;
}


DWGraph_statusCodes dwgraph_add_edge(DWGraph graph, void* srcNode, void* dstNode, void* weight) {
    //tests di robustezza
    if (graph == NULL || srcNode == NULL || dstNode == NULL || weight == NULL) return DWGraph_ERROR_NULL_POINTER;

    if (graph->numEdges == (graph->edgesCapacity -1) ) {

        size_t newSize = graph->edgesCapacity * GROWTH_FACTOR;
        if (newSize < graph->edgesCapacity) return DWGraph_ERROR_ARITHMETIC_OVERFLOW;
        DWGraph_edge* temp = realloc(graph->edges, newSize);
        if (temp == NULL) return DWGraph_ERROR_ALLOCATION_FAILURE;

        graph->edges = temp;
    }

    graph->edges[graph->numEdges].from = srcNode;
    graph->edges[graph->numEdges].to = dstNode;
    graph->edges[graph->numEdges].weight = weight;
    graph->numEdges +=1;

    return DWGraph_OK;
}