#include <string>
#include <memory>
#include <vector>

class DistCalculator{
public:
  using Node = uint64_t;
  DistCalculator(std::string edgeListFile);
  int64_t dist(Node a, Node b);
  void bfs(bool *visitedActors, bool *otherVisitedActors, bool *visitedMovies, std::vector<int> *swapQueue, std::vector<int>* actorQueue, bool a);

  int a_dist;
  int b_dist;
  bool end;
  std::vector<std::vector<int>> actorMovies;
  std::vector<std::vector<int>> movieActors;
};
