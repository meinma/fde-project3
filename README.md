# FDE Project 3
This is the implementation of the third project of the course Foundations of Data Engineerinf offered by TU München. It's about searching in social graphs. The implementation is in the function DistCalculator::dist(Node a, Node b) receiving two actor ids and must return the distance.


## Dataset
The playedin.csv file is used as the dataset and consists of two columns:
1. actor id (integer)
2. movie id (integer)
The integer values form a dense range starting from 0 to the number of elements enumerated.

## Task
Based on that graph and two actor ids, the program must compute the length of the shortest coworking chain between two actors. Chains are built of actors that acted in the same movie. For example if Kevin Bacon and Dustin Hoffman worked on the same movie, the chain length between them is 1. If Dustin Hoffman then worked with Barbra Streisand, but Barbra Streisand did not work with Kevin Bacon, the shortest chain length is 2. This extends to arbitrary chain lengths. If no chain can be found, the program should return -1. The chain length from an actor to himself is defined to be 0.

## Time limit
The implementation has to complete the workload in less than 20 seconds.