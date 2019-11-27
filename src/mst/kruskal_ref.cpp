//
// Created by Wenting Ye on 11/26/19.
//
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include "graph.h"

namespace boost {
    typedef adjacency_list <vecS, vecS, undirectedS,
    no_property, property<edge_weight_t, double>> Graph;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef std::pair<int, int> E;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid command: bin/kruskal_ref <data_path>"
                  << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::cout << "File name: " << filename << std::endl;
    struct timeval start, end;
    double duration;

    // Load the graph.
    gettimeofday(&start, nullptr);
    Graph graph;
    graph.loadGraph(filename);

    const int num_nodes = graph.num_vertices;
    std::vector<boost::E> edge_array;
    std::vector<double> weights;
    for (auto &edge : graph.edges) {
        edge_array.emplace_back(edge.from, edge.to);
        weights.push_back(edge.weight);
    }
    boost::Graph g(edge_array.begin(), edge_array.end(), weights.begin(),
                   num_nodes);
    boost::property_map<boost::Graph, boost::edge_weight_t>::type weight = get(
            boost::edge_weight, g);

    gettimeofday(&end, nullptr);
    duration = (double) ((end.tv_sec - start.tv_sec) * 1000000u +
                         end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "loading time: " << duration << " s." << std::endl;

    // Run the MST algorithm.
    gettimeofday(&start, nullptr);
    std::vector<boost::Edge> spanning_tree;
    kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
    gettimeofday(&end, nullptr);
    duration = (double) ((end.tv_sec - start.tv_sec) * 1000000u +
                         end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "MST time: " << duration << " s." << std::endl;

    // Save the results.
    std::string output_filename = filename + ".ref.output";
    std::cout << "Saving MST to " << output_filename << "." << std::endl;
    std::vector<Edge> mst_edges;
    for (std::vector<boost::Edge>::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        mst_edges.push_back({source(*ei, g), target(*ei, g), weight[*ei]});
    }
    graph.edges = mst_edges;
    graph.saveGraph(output_filename);

    return EXIT_SUCCESS;
}