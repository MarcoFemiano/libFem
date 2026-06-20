//
// Created by Spara on 20/06/2026.
//

#ifndef LIBFEM_DIRECT_WGRAPH_H
#define LIBFEM_DIRECT_WGRAPH_H

typedef char bool;
#define true 1
#define false 0

typedef enum {
    DWGraph_OK = 0,
    DWGraph_ERROR_NULL_POINTER = -1,
    DWGraph_ERROR_ALLOCATION_FAILURE = -2,
    DWGraph_ERROR_SEMANTIC_FAILURE = -3,
    DWGraph_ERROR_ARITHMETIC_OVERFLOW = -4
}DWGraph_statusCodes;


typedef struct strDWGraph* DWGraph;


DWGraph DWGraph_create(void);

DWGraph_statusCodes dwgraph_destroy(DWGraph* graph);

DWGraph_statusCodes dwgraph_add_node(DWGraph graph, void* node);
DWGraph_statusCodes dwgraph_add_edge(DWGraph graph, void* srcNode, void* dstNode, void* weight);




#endif //LIBFEM_DIRECT_WGRAPH_H