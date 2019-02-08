#include "DistCalculator.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>

DistCalculator::DistCalculator(std::string edgeListFile)
{
   std::ifstream stream;
   stream.open(edgeListFile, std::ios::in);
   if (stream.is_open()) {
      int actorId = 0;
      int movieId = 0;
      int maxActorId = 0;
      int maxMovieId = 0;
      std::string line;
      while (std::getline(stream, line)) maxActorId++;
      maxMovieId = maxActorId;

      actorMovies.resize(std::max(maxActorId + 1, 50));
      movieActors.resize(std::max(maxMovieId + 1, 50));
      stream.clear();
      stream.seekg(0, std::ios::beg);
      int counter = 0;
      // Skip first line
      std::getline(stream, line);
      // Read line by line of the input file
      while (std::getline(stream, line)) {
         // Get the actorId of the line
         const char *iter = line.data(), *limit = iter + line.length();
         unsigned v = 0;
         for (; *iter != ','; ++iter) v = 10 * v + (*iter) - '0';
         actorId = v;
         // Skip ','
         iter++;
         v = 0;
         // Get the movieId of the line
         for (; iter != limit; ++iter) v = 10 * v + (*iter) - '0';
         movieId = v;

         actorMovies[actorId].push_back(movieId);
         movieActors[movieId].push_back(actorId);
      }
      // Nach dem ersten Durchlauf
   }
}


// Node a und b sind nur Zahlen
int64_t DistCalculator::dist(Node a, Node b) {
    if (a == b)
        return 0;

    // Alles für Bidirektionale BFS von a aus
    bool *a_visitedActors = new bool[actorMovies.size() + 1];
    for (int i = 0; i < actorMovies.size(); i++)
        a_visitedActors[i] = false;
    //speichert alle Movies, um mehrmaliges Besuchen zu verhindern
    bool *a_visitedMovies = new bool[movieActors.size() + 1];
    for (int i = 0; i < movieActors.size(); i++)
        a_visitedMovies[i] = false;
    //Initialisieren der Distanz
    a_dist = 0;
    std::vector<int> a_swapQueue;
    a_swapQueue.push_back(a);
    a_visitedActors[a] = true;

    // Alles für Bidirektionale BFS von b aus
    bool *b_visitedActors = new bool[actorMovies.size() + 1];
    for (int i = 0; i < actorMovies.size(); i++)
        b_visitedActors[i] = false;
    //speichert alle Movies, um mehrmaliges Besuchen zu verhindern
    bool *b_visitedMovies = new bool[movieActors.size() + 1];
    for (int i = 0; i < movieActors.size(); i++)
        b_visitedMovies[i] = false;
    //Initialisieren der Distanz
    b_dist = 0;
    std::vector<int> b_swapQueue;
    b_swapQueue.push_back(b);
    b_visitedActors[b] = true;

    while (!a_swapQueue.empty() && !b_swapQueue.empty() && a_dist+b_dist < 6) {
      bfs(a_visitedActors,a_visitedMovies,&a_swapQueue,0);
      if (interselect(a_visitedActors,b_visitedActors))
          return a_dist + b_dist;
      bfs(b_visitedActors,b_visitedMovies,&b_swapQueue,1);
      if (interselect(a_visitedActors,b_visitedActors))
          return a_dist + b_dist;
    }
    return -1;

}


void DistCalculator::bfs(bool *visitedActors, bool *visitedMovies, std::vector<int> *swapQueue, bool a) {
    std::vector<int> actorQueue;
    if (a)
        a_dist++;
    else
        b_dist++;
    std::swap(actorQueue, *swapQueue);
    swapQueue->clear();
    int s = actorQueue.front();
    for (auto &movie: actorMovies[s]) {
        if (visitedMovies[movie]) //besuchte movies werden übersprungen
            continue;
        visitedMovies[movie] = true; //movies auf besucht setzen
        for (auto &actor : movieActors[movie]) {
            if (!visitedActors[actor]) {
                swapQueue->push_back(actor);
                visitedActors[actor] = true;
            }
        }
    }
    actorQueue.clear();
}


// Für bidirektionale BFS notwenidg um zu sehen, wenn beide BFS aufeinander treffen
bool DistCalculator::interselect(bool *s_visited, bool *t_visited) {
    for (int i = 0; i < actorMovies.size(); i++) {
        if (s_visited[i] && t_visited[i])
            return true;
    }
    return false;
}

