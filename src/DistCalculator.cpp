#include "DistCalculator.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>

DistCalculator::DistCalculator(std::string edgeListFile) {
    std::ifstream stream;
    stream.open(edgeListFile, std::ios::in);
    if (stream.is_open()) {
        int actorId = 0;
        int movieId = 0;
        std::unordered_set<int> actors;
        std::unordered_set<int> movies;
        std::string line;
        for (int i = 0; i < 2; i++) {
            int counter = 0;
            // Read line by line of the input file
            while (std::getline(stream, line)) {
                // Skip the first line because there's no relevant data
                if (counter == 0) {
                    counter++;
                    continue;
                }

                // Get the actorId of the line --> funktioniert korrekt
                const char *data = line.data(), *limit = data + line.length(), *last = data;
                unsigned field = 0;
                for (auto iter = data; iter != limit; ++iter) {
                    if ((*iter) == ',') {
                        if (++field == 1) {
                            union {
                                unsigned v;
                                char buffer[sizeof(unsigned)];
                            };
                            v = 0;
                            for (auto iter2 = last; iter2 != iter; ++iter2)
                                v = 10 * v + (*iter2) - '0';
                            actorId = v;
                            break;

                        } else
                            last = iter + 1;
                    }
                }

                const char *data2 = line.data(), *limit2 = data + line.length(), *last2 = data;
                unsigned field2 = 0;
                for (auto iter3 = data2; iter3 != limit2; ++iter3) {
                    if ((*iter3) == ',') {
                        if (++field2 == 1) {
                            std::string w(iter3);
                            movieId = std::stoi(w.substr(1, w.size()));
                            break;
                        }
                    }
                }

                //Jede ActorId wird gespeichert um später die Anzahl der unterschiedlichen Actors zu haben im ersten Durchgang
                if (i == 0) {
                    actors.insert(actorId);
                    movies.insert(movieId);
                }
                    // Im zweiten Durchgang füllen der Vectoren
                else {
                    actorMovies[actorId].push_back(movieId);
                    movieActors[movieId].push_back(actorId);
                }
            }
            // Nach dem ersten Durchlauf
            if (i == 0) {
                int distinctActors = actors.size();
                int distinctMovies = movies.size();
                actorMovies.resize(distinctActors + 1);
                movieActors.resize(distinctMovies + 1);
                stream.clear();
                stream.seekg(0, std::ios::beg);
            }
        }
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

