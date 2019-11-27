/**
 * @file graph_generator.cpp
 * @brief Graph data generator.
 *
 * @author Xuren Zhou (xurenz)
 * @bug No known bugs.
 */

#include <cassert>
#include <iostream>
#include <random>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    // Parse the arguement.
    if (argc != 3) {
        cerr << "graph_generator <num_nodes> <edge_prob>" << endl;
        return 1;
    }

    int num_nodes = atoi(argv[1]);
    double prob_thre_edge = stod(argv[2]);
    double upper_weight = 1.0;
    double lower_weight = 10.0;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> edge_dist(0, 1);
    uniform_real_distribution<double> weight_dist(upper_weight, lower_weight);

    Graph src_graph, dst_graph;

    src_graph.num_vertices = num_nodes;

    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            if (edge_dist(mt) < prob_thre_edge) {
                src_graph.edges.push_back({i, j, weight_dist(mt)});
            }
        }
    }

    string filename = "./data/random_" + to_string(num_nodes) + "_" + to_string(prob_thre_edge);
    src_graph.saveGraph(filename);

    dst_graph.loadGraph(filename);

    assert(src_graph.num_vertices == dst_graph.num_vertices);
    assert(src_graph.edges.size() == dst_graph.edges.size());

    for (size_t i = 0; i < src_graph.edges.size(); i++) {
        assert(src_graph.edges[i].from == dst_graph.edges[i].from);
        assert(src_graph.edges[i].to == dst_graph.edges[i].to);
        assert((src_graph.edges[i].weight - dst_graph.edges[i].weight) < 0.1);
    }

    cout << "#node: " << src_graph.num_vertices << " #edge: " << src_graph.edges.size() << endl;
    cout << "Save to " << filename << endl;

    return 0;
}




