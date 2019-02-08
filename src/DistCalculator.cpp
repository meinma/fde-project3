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
   }
}


// Node a und b sind nur Zahlen
int64_t DistCalculator::dist(Node a, Node b) {
    if (a == b)
        return 0;
    int a_dist;
    int b_dist;
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
    b_dist = 0;
    std::vector<int> a_swapQueue;
    std::vector<int> a_actorQueue;
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
    std::vector<int> b_actorQueue;
    b_swapQueue.push_back(b);
    b_visitedActors[b] = true;

    while (!a_swapQueue.empty() && !b_swapQueue.empty() && a_dist+b_dist < 6) {
      auto end = bfs(a_visitedActors,b_visitedActors,a_visitedMovies,&a_swapQueue,&a_actorQueue,0, a_dist, b_dist);
      if (end)
          return a_dist + b_dist;
      end = bfs(b_visitedActors,a_visitedActors,b_visitedMovies,&b_swapQueue,&b_actorQueue,1, a_dist, b_dist);
      if (end)
          return  a_dist + b_dist;
    }
    return -1;
}


bool DistCalculator::bfs(bool *visitedActors, bool *otherVisitedActors, bool *visitedMovies, std::vector<int> *swapQueue, std::vector<int> *actorQueue, bool a,
      int& a_dist, int& b_dist) {
    if (a)
        a_dist++;
    else
        b_dist++;
    std::swap(*actorQueue, *swapQueue);
    swapQueue->clear();
    int s = actorQueue->front();
    for (auto &movie: actorMovies[s]) {
        if (visitedMovies[movie]) //besuchte movies werden übersprungen
            continue;
        visitedMovies[movie] = true; //movies auf besucht setzen
        for (auto &actor : movieActors[movie]) {
            if (!visitedActors[actor] && !otherVisitedActors[actor]) {
                swapQueue->push_back(actor);
                visitedActors[actor] = true;
            }
            else if (otherVisitedActors[actor])
               return true;
        }
    }
    actorQueue->clear();
    return false;
}


