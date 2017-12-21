#include <stdexcept>
#include <vector>

#include "test_framework/test_timer.h"
#include "test_framework/test_utils_serialization_traits.h"

using std::vector;

struct GraphVertex {
  enum Color { white, gray, black } color = white;
  vector<GraphVertex*> edges;
};

bool IsDeadlocked(vector<GraphVertex>* G) {
  // Implement this placeholder.
  return true;
}

struct Edge {
  int from;
  int to;
};

template <>
struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

bool HasCycleWrapper(TestTimer& timer, int k, const vector<Edge>& edges) {
  vector<GraphVertex> g;
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }
  g.reserve(k);

  for (int i = 0; i < k; i++) {
    g.push_back(GraphVertex{});
  }

  for (auto& e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    g[e.from].edges.push_back(&g[e.to]);
  }

  timer.Start();
  bool result = IsDeadlocked(&g);
  timer.Stop();
  return result;
}

#include "test_framework/test_utils_generic_main.h"

int main(int argc, char* argv[]) {
  generic_test_main(argc, argv, "deadlock_detection.tsv", &HasCycleWrapper);
  return 0;
}