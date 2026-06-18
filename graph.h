/**
 * @file graph.h
 * @brief ADT grafo con liste di adiacenza e algoritmi classici.
 *
 * Il grafo ha vertici numerati da 0 a `vertices - 1`. Gli archi hanno peso
 * intero non negativo. Il modulo include BFS, DFS, Dijkstra e topological sort.
 */

#ifndef LIBFEM_GRAPH_H
#define LIBFEM_GRAPH_H

#include "libfem_common.h"

#include <limits.h>

/** @brief Distanza usata da Dijkstra per vertici non raggiungibili. */
#define GRAPH_DISTANCE_INF (INT_MAX / 4)

/** @brief Handle opaco del grafo. */
typedef struct strGraph* Graph;

/** @brief Crea un grafo diretto o non diretto con `vertices` vertici. */
status_codes graph_create(Graph* graph, size_t vertices, bool directed);

/** @brief Distrugge il grafo e tutte le liste di adiacenza. */
status_codes graph_destroy(Graph* graph);

/** @brief Aggiunge un arco con peso non negativo. */
status_codes graph_add_edge(Graph graph, size_t from, size_t to, int weight);

/** @brief Esegue BFS da `start` e scrive l'ordine di visita. */
status_codes graph_bfs(Graph graph, size_t start, size_t* order, size_t orderCapacity, size_t* resultCount);

/** @brief Esegue DFS da `start` e scrive l'ordine di visita. */
status_codes graph_dfs(Graph graph, size_t start, size_t* order, size_t orderCapacity, size_t* resultCount);

/** @brief Calcola le distanze minime da `start` con Dijkstra. */
status_codes graph_dijkstra(Graph graph, size_t start, int* distances, size_t distancesCapacity);

/** @brief Calcola un ordinamento topologico di un grafo diretto aciclico. */
status_codes graph_topological_sort(Graph graph, size_t* order, size_t orderCapacity, size_t* resultCount);

/** @brief Restituisce il numero di vertici. */
status_codes graph_vertex_count(Graph graph, size_t* result);

/** @brief Restituisce il numero di archi logici aggiunti. */
status_codes graph_edge_count(Graph graph, size_t* result);

#endif /* LIBFEM_GRAPH_H */
