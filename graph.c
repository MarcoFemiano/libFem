//
// Created by Spara on 03/06/2026.
//

#include "graph.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct strGraphEdge* GraphEdge;

struct strGraphEdge {
  size_t to;
  int weight;
  GraphEdge next;
};

struct strGraph {
  GraphEdge* adjacency;
  size_t vertices;
  size_t edges;
  bool directed;
};

static void graph_destroy_edges(GraphEdge edge) {
  while (edge != NULL) {
    GraphEdge next = edge->next;
    free(edge);
    edge = next;
  }
}

static GraphEdge graph_create_edge(size_t to, int weight) {
  GraphEdge edge = malloc(sizeof(struct strGraphEdge));
  if (edge == NULL) return NULL;

  edge->to = to;
  edge->weight = weight;
  edge->next = NULL;
  return edge;
}

static void graph_link_edge(Graph graph, size_t from, GraphEdge edge) {
  edge->next = graph->adjacency[from];
  graph->adjacency[from] = edge;
}

static status_codes graph_validate_visit_buffers(Graph graph, size_t start, size_t* order, size_t orderCapacity, size_t* resultCount) {
  //tests di robustezza
  if (graph == NULL || order == NULL || resultCount == NULL) return ERROR_NULL_POINTER;
  if (start >= graph->vertices) return ERROR_OUT_OF_BOUNDS;
  if (orderCapacity < graph->vertices) return ERROR_INVALID_ARGUMENT;

  return OK;
}

static status_codes graph_dfs_visit(Graph graph, size_t vertex, bool* visited, size_t* order, size_t* resultCount) {
  //tests di robustezza
  if (graph == NULL || visited == NULL || order == NULL || resultCount == NULL) return ERROR_NULL_POINTER;

  visited[vertex] = true;
  order[*resultCount] = vertex;
  (*resultCount)++;

  GraphEdge edge = graph->adjacency[vertex];
  while (edge != NULL) {
    if (!visited[edge->to]) {
      status_codes res = graph_dfs_visit(graph, edge->to, visited, order, resultCount);
      if (res != OK) return res;
    }
    edge = edge->next;
  }

  return OK;
}

status_codes graph_create(Graph* graph, size_t vertices, bool directed) {
  //tests di robustezza
  if (graph == NULL) return ERROR_NULL_POINTER;
  if (vertices == 0) return ERROR_INVALID_ARGUMENT;
  if (vertices > SIZE_MAX / sizeof(GraphEdge)) return ERROR_ARITHMETIC_OVERFLOW;

  Graph temp = malloc(sizeof(struct strGraph));
  if (temp == NULL) return ERROR_ALLOCATION_FAILURE;

  temp->adjacency = calloc(vertices, sizeof(GraphEdge));
  if (temp->adjacency == NULL) {
    free(temp);
    return ERROR_ALLOCATION_FAILURE;
  }

  temp->vertices = vertices;
  temp->edges = 0;
  temp->directed = directed;

  *graph = temp;
  return OK;
}

status_codes graph_destroy(Graph* graph) {
  //tests di robustezza
  if (graph == NULL || *graph == NULL) return ERROR_NULL_POINTER;

  for (size_t i = 0; i < (*graph)->vertices; i++) {
    graph_destroy_edges((*graph)->adjacency[i]);
  }

  free((*graph)->adjacency);
  free(*graph);
  *graph = NULL;

  return OK;
}

status_codes graph_add_edge(Graph graph, size_t from, size_t to, int weight) {
  //tests di robustezza
  if (graph == NULL) return ERROR_NULL_POINTER;
  if (from >= graph->vertices || to >= graph->vertices) return ERROR_OUT_OF_BOUNDS;
  if (weight < 0) return ERROR_INVALID_ARGUMENT;

  GraphEdge first = graph_create_edge(to, weight);
  if (first == NULL) return ERROR_ALLOCATION_FAILURE;

  GraphEdge second = NULL;
  if (!graph->directed) {
    second = graph_create_edge(from, weight);
    if (second == NULL) {
      free(first);
      return ERROR_ALLOCATION_FAILURE;
    }
  }

  graph_link_edge(graph, from, first);
  if (!graph->directed) graph_link_edge(graph, to, second);
  graph->edges++;

  return OK;
}

status_codes graph_bfs(Graph graph, size_t start, size_t* order, size_t orderCapacity, size_t* resultCount) {
  status_codes res = graph_validate_visit_buffers(graph, start, order, orderCapacity, resultCount);
  if (res != OK) return res;

  *resultCount = 0;

  bool* visited = calloc(graph->vertices, sizeof(bool));
  if (visited == NULL) return ERROR_ALLOCATION_FAILURE;

  size_t* queue = malloc(graph->vertices * sizeof(size_t));
  if (queue == NULL) {
    free(visited);
    return ERROR_ALLOCATION_FAILURE;
  }

  size_t head = 0;
  size_t tail = 0;
  queue[tail++] = start;
  visited[start] = true;

  while (head < tail) {
    size_t vertex = queue[head++];
    order[*resultCount] = vertex;
    (*resultCount)++;

    GraphEdge edge = graph->adjacency[vertex];
    while (edge != NULL) {
      if (!visited[edge->to]) {
        visited[edge->to] = true;
        queue[tail++] = edge->to;
      }
      edge = edge->next;
    }
  }

  free(queue);
  free(visited);
  return OK;
}

status_codes graph_dfs(Graph graph, size_t start, size_t* order, size_t orderCapacity, size_t* resultCount) {
  status_codes res = graph_validate_visit_buffers(graph, start, order, orderCapacity, resultCount);
  if (res != OK) return res;

  *resultCount = 0;

  bool* visited = calloc(graph->vertices, sizeof(bool));
  if (visited == NULL) return ERROR_ALLOCATION_FAILURE;

  res = graph_dfs_visit(graph, start, visited, order, resultCount);

  free(visited);
  return res;
}

status_codes graph_dijkstra(Graph graph, size_t start, int* distances, size_t distancesCapacity) {
  //tests di robustezza
  if (graph == NULL || distances == NULL) return ERROR_NULL_POINTER;
  if (start >= graph->vertices) return ERROR_OUT_OF_BOUNDS;
  if (distancesCapacity < graph->vertices) return ERROR_INVALID_ARGUMENT;

  bool* visited = calloc(graph->vertices, sizeof(bool));
  if (visited == NULL) return ERROR_ALLOCATION_FAILURE;

  for (size_t i = 0; i < graph->vertices; i++) {
    distances[i] = GRAPH_DISTANCE_INF;
  }
  distances[start] = 0;

  for (size_t step = 0; step < graph->vertices; step++) {
    size_t bestVertex = graph->vertices;
    int bestDistance = GRAPH_DISTANCE_INF;

    for (size_t i = 0; i < graph->vertices; i++) {
      if (!visited[i] && distances[i] < bestDistance) {
        bestDistance = distances[i];
        bestVertex = i;
      }
    }

    if (bestVertex == graph->vertices) break;

    visited[bestVertex] = true;
    GraphEdge edge = graph->adjacency[bestVertex];
    while (edge != NULL) {
      if (distances[bestVertex] <= GRAPH_DISTANCE_INF - edge->weight) {
        int candidate = distances[bestVertex] + edge->weight;
        if (candidate < distances[edge->to]) distances[edge->to] = candidate;
      }
      edge = edge->next;
    }
  }

  free(visited);
  return OK;
}

status_codes graph_topological_sort(Graph graph, size_t* order, size_t orderCapacity, size_t* resultCount) {
  //tests di robustezza
  if (graph == NULL || order == NULL || resultCount == NULL) return ERROR_NULL_POINTER;
  if (!graph->directed) return ERROR_INVALID_ARGUMENT;
  if (orderCapacity < graph->vertices) return ERROR_INVALID_ARGUMENT;

  *resultCount = 0;

  size_t* indegree = calloc(graph->vertices, sizeof(size_t));
  if (indegree == NULL) return ERROR_ALLOCATION_FAILURE;

  size_t* queue = malloc(graph->vertices * sizeof(size_t));
  if (queue == NULL) {
    free(indegree);
    return ERROR_ALLOCATION_FAILURE;
  }

  for (size_t i = 0; i < graph->vertices; i++) {
    GraphEdge edge = graph->adjacency[i];
    while (edge != NULL) {
      indegree[edge->to]++;
      edge = edge->next;
    }
  }

  size_t head = 0;
  size_t tail = 0;
  for (size_t i = 0; i < graph->vertices; i++) {
    if (indegree[i] == 0) queue[tail++] = i;
  }

  while (head < tail) {
    size_t vertex = queue[head++];
    order[*resultCount] = vertex;
    (*resultCount)++;

    GraphEdge edge = graph->adjacency[vertex];
    while (edge != NULL) {
      indegree[edge->to]--;
      if (indegree[edge->to] == 0) queue[tail++] = edge->to;
      edge = edge->next;
    }
  }

  free(queue);
  free(indegree);

  if (*resultCount != graph->vertices) return ERROR_GRAPH_HAS_CYCLE;
  return OK;
}

status_codes graph_vertex_count(Graph graph, size_t* result) {
  //tests di robustezza
  if (graph == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = graph->vertices;
  return OK;
}

status_codes graph_edge_count(Graph graph, size_t* result) {
  //tests di robustezza
  if (graph == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = graph->edges;
  return OK;
}
