//
// Created by Wenting Ye on 11/14/19.
//
#include <algorithm>
#include <iostream>
#include <sys/time.h>
#include "disjoint_set.h"
#include "graph.h"

bool compareByWeight(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

Graph kruskalMSTSequential(const Graph &graph) {
    struct timeval start, end;
    double duration, total;

    total = 0;
    // Copy and sort the edges by weight.
    std::vector<Edge> edges = graph.edges;
    gettimeofday(&start, nullptr);
    std::sort(edges.begin(), edges.end(), compareByWeight);
    gettimeofday(&end, nullptr);
    duration = (double)((end.tv_sec  - start.tv_sec) * 1000000u +
                        end.tv_usec - start.tv_usec) / 1.e6;
    total += duration;
    std::cout << "Sort time: " << duration << " s." << std::endl;

    // Kruskal algorithm.
    gettimeofday(&start, nullptr);
    DisjointSet ds(graph.num_vertices);
    std::vector<Edge> mst_edges;
    for (const Edge e : edges) {
        if (!ds.belongSameSet(e.from, e.to)) {
            ds.unionSet(e.from, e.to);
            mst_edges.push_back(e);
        }
    }
    gettimeofday(&end, nullptr);
    duration = (double) ((end.tv_sec - start.tv_sec) * 1000000u +
                         end.tv_usec - start.tv_usec) / 1.e6;
    total += duration;
    std::cout << "Merging time: " << duration << " s." << std::endl;
    std::cout << "total time: " << total << " s." << std::endl;

    return {graph.num_vertices, mst_edges};
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid command: bin/kruskal_seq <data_path>"
                  << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::cout << "File name: " << filename << std::endl;
    struct timeval start, end;
    double duration;

    gettimeofday(&start, nullptr);
    Graph graph;
    graph.loadGraph(filename);
    gettimeofday(&end, nullptr);
    duration = (double) ((end.tv_sec - start.tv_sec) * 1000000u +
                         end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "loading time: " << duration << " s." << std::endl;

    // Execute the algorithm and print the MST.
    Graph mst = kruskalMSTSequential(graph);

    std::string output_filename = filename + ".seq.output";
    std::cout << "Saving MST to " << output_filename << "." << std::endl;
    mst.saveGraph(output_filename);

    return 0;
}
