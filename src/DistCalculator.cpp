#include "DistCalculator.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <list>

DistCalculator::DistCalculator(std::string edgeListFile) {
    std::ifstream stream;
    stream.open(edgeListFile, std::ios::in);
    if (stream.is_open()) {
        int actorId = 0;
        int movieId = 0;
        std::unordered_set<int> actors;
        std::unordered_set<int> movies;
        std::string line;
        for (int i  = 0; i < 2; i++) {
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
                            movieId = std::stoi(w.substr(1,w.size()));
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
                else{
                    actorMovies[actorId].push_back(movieId);
                    movieActors[movieId].push_back(actorId);
                }
            }
            // Nach dem ersten Durchlauf
            if (i == 0) {
                int distinctActors = actors.size() ;
                int distinctMovies = movies.size();
                actorMovies.resize(distinctActors + 1);
                movieActors.resize(distinctMovies + 1);
                stream.clear();
                stream.seekg(0,std::ios::beg);
            }
        }
    }
}

// Node a und b sind nur Zahlen
int64_t DistCalculator::dist(Node a, Node b) {
    bool * visited = new bool[actorMovies.size()];
    for (int i = 0; i < actorMovies.size(); i++)
        visited[i] = false;
    int distance = 0;
    if (a == b)
        return distance;
    else {
        std::list<int>actorQueue;
        std::list<int>swapQueue;
        swapQueue.push_back(a);
        int s;
        while(!swapQueue.empty() && distance < 6) {
            if (actorQueue.empty()){
                distance++;
                actorQueue = swapQueue;
                swapQueue.clear();
            }
            s = actorQueue.front();
            visited[s] = true;
            for (int i = 0; i < actorMovies[s].size(); i++){
                for (int j = 0; j < movieActors[actorMovies[s][i]].size(); j++) {
                    int elem = movieActors[actorMovies[s][i]][j];
                    if ((b != elem) && (!visited[elem]))
                        swapQueue.push_back(elem);
                    else if (b == elem)
                        return distance;
                }
            }
            actorQueue.clear();
        }
    }
    return -1;

}


