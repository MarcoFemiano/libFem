#include "graph.h"
#include "test_support.h"

int main(void) {
  Graph graph = NULL;
  size_t order[5];
  size_t count = 0;
  int distances[5];

  test_expect_int("create ok", graph_create(&graph, 5, true), OK);
  test_expect_int("edge 0->1", graph_add_edge(graph, 0, 1, 2), OK);
  test_expect_int("edge 0->2", graph_add_edge(graph, 0, 2, 5), OK);
  test_expect_int("edge 1->2", graph_add_edge(graph, 1, 2, 1), OK);
  test_expect_int("edge 1->3", graph_add_edge(graph, 1, 3, 4), OK);
  test_expect_int("edge 2->3", graph_add_edge(graph, 2, 3, 1), OK);
  test_expect_int("edge 3->4", graph_add_edge(graph, 3, 4, 3), OK);

  test_expect_int("bfs", graph_bfs(graph, 0, order, 5, &count), OK);
  test_expect_size("bfs count", count, 5);
  test_expect_int("bfs starts 0", (int)order[0], 0);

  test_expect_int("dfs", graph_dfs(graph, 0, order, 5, &count), OK);
  test_expect_size("dfs count", count, 5);

  test_expect_int("dijkstra", graph_dijkstra(graph, 0, distances, 5), OK);
  test_expect_int("dist 0", distances[0], 0);
  test_expect_int("dist 2", distances[2], 3);
  test_expect_int("dist 4", distances[4], 7);

  test_expect_int("topological", graph_topological_sort(graph, order, 5, &count), OK);
  test_expect_size("topological count", count, 5);
  test_expect_int("destroy", graph_destroy(&graph), OK);

  test_expect_int("create cycle graph", graph_create(&graph, 2, true), OK);
  test_expect_int("cycle edge 0", graph_add_edge(graph, 0, 1, 1), OK);
  test_expect_int("cycle edge 1", graph_add_edge(graph, 1, 0, 1), OK);
  test_expect_int("cycle rejected", graph_topological_sort(graph, order, 5, &count), ERROR_GRAPH_HAS_CYCLE);
  test_expect_int("destroy cycle", graph_destroy(&graph), OK);

  return test_result();
}
