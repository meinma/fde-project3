#include <string>
#include <memory>
#include <vector>

class DistCalculator{
public:
  using Node = uint64_t;
  DistCalculator(std::string edgeListFile);
  int64_t dist(Node a, Node b);
  bool interselect(bool* a_visited,bool* b_visited);
  void bfs(bool *visitedActors, bool *visitedMovies, std::vector<int> *swapQueue, bool a);

  int a_dist;
  int b_dist;
  std::vector<std::vector<int>> actorMovies;
  std::vector<std::vector<int>> movieActors;
};
