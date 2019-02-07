#include <string>
#include <memory>
#include <vector>

class DistCalculator{
public:
  using Node = uint64_t;
  DistCalculator(std::string edgeListFile);
  int64_t dist(Node a, Node b);

  std::vector<std::vector<int>> actorMovies;
  std::vector<std::vector<int>> movieActors;
};
