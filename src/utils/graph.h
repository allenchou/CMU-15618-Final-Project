//
// Created by Xuren Zhou on 11/12/19.
//

#ifndef MST_GRAPH_H
#define MST_GRAPH_H

#include <unordered_map>
#include <utility>
#include <vector>

// Undirected edge.
struct Edge {
    int from;
    int to;
    double weight;
};

// Undirected graph.
struct Graph {
    Graph() = default;

    Graph(const int num_vertices, std::vector<Edge> edges): num_vertices(num_vertices), edges(std::move(edges)) {}

    void printGraph();

    void loadGraph(const std::string& filename);

    void saveGraph(const std::string& filename);

    int num_vertices;
    std::vector<Edge> edges;
};


#endif //MST_GRAPH_H
