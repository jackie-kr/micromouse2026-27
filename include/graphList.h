// C++ Program to Implement a Graph Using Adjacency List
#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;

class GraphList {
    unordered_map<int, vector<int> >
        adjList; // Adjacency list to store the graph

public:
    unordered_map<int, vector<int>>& getList() {
        return adjList;
    }

    // Function to add an edge between vertices u and v of
    // the graph
    void add_edge(int u, int v)
    {
        // Add edge from u to v
        adjList[u].push_back(v);
        // Add edge from v to u because the graph is
        // undirected
        adjList[v].push_back(u);
    }

    // Function to print the adjacency list representation
    // of the graph
    void print() const
    {
        cerr << "Adjacency list for the Graph: " << endl;
        // Iterate over each vertex
        for (auto i : adjList) {
            // Print the vertex
            cerr << i.first << " -> ";
            // Iterate over the connected vertices
            for (auto j : i.second) {
                // Print the connected vertex
                cerr << j << " ";
            }
            cerr << endl;
        }
    }
};
