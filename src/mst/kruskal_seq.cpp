#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include "disjoint_set.h"
#include "graph.h"

bool compareByWeight(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

Graph kruskalMSTSequential(const Graph &graph) {
    // Copy and sort the edges by weight.
    std::vector<Edge> edges = graph.edges;
    clock_t start = clock();
    std::sort(edges.begin(), edges.end(), compareByWeight);
    clock_t end = clock();
    double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    std::cout << "Sort time: " << time << "ms." << std::endl;

    // Kruskal algorithm.
    start = clock();
    DisjointSet ds(graph.num_vertices);
    std::vector<Edge> mst_edges;
    for (const Edge e : edges) {
        if (!ds.belongSameSet(e.from, e.to)) {
            ds.unionSet(e.from, e.to);
            mst_edges.push_back(e);
        }
    }
    end = clock();
    time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    std::cout << "Merging time: " << time << "ms." << std::endl;

    return {graph.num_vertices, mst_edges};
}

int main() {
    clock_t start = clock();
    std::string filename = "demo_5.txt";
    std::cout << "File name: " << filename << std::endl;
    std::ifstream fin;
    fin.open(filename);

    // Build the graph from input.
    int num_vertices;
    std::vector<Edge> edges;
    fin >> num_vertices;
    while (!fin.eof()) {
        int from, to;
        double w;
        fin >> from >> to >> w;
        edges.push_back({from, to, w});
    }

    clock_t end = clock();
    double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    std::cout << "Loading time: " << time << "ms." << std::endl;

    // Execute the algorithm and print the MST.
    Graph mst = kruskalMSTSequential({num_vertices, edges});
    mst.printGraph();

    return 0;
}