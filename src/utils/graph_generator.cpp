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

static const double upper_weight = 1.0;
static const double lower_weight = 10.0;

using namespace std;

int powerLawDegree(double x0, double x1, double n, double y) {
    auto lower = pow(x0, n+1);
    auto upper = pow(x1, n+1);
    auto x = pow((upper - lower) * y + lower, 1.0 / (n+1));
    return static_cast<int>(x);
}

Graph powerLawGraph(int num_nodes, double power) {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> y(0, 1);
    uniform_int_distribution<int> edge_idx(0, num_nodes-1);
    uniform_real_distribution<double> weight_dist(upper_weight, lower_weight);

    Graph graph;
    graph.num_vertices = num_nodes;

    vector<Edge> dup_edges;
    vector<bool> marked(num_nodes, false);
    for (int i = 0; i < num_nodes; i++) {
        // init
        for (int j = 0; j < num_nodes; j++) {
            marked[j] = false;
        }

        // degree
        int degree = powerLawDegree(1, num_nodes - 1, power, y(mt));

        // set mark
        for (int j = 0; j < degree; j++) {
            marked[edge_idx(mt)] = true;
        }

        // set edge
        for (int j = 0; j < num_nodes; j++) {
            if (j != i && marked[j]) {
                dup_edges.push_back({std::min(i, j), std::max(i, j), weight_dist(mt)});
            }
        }
    }

    // dedup
    std::sort(dup_edges.begin(), dup_edges.end(), [](const Edge& lhs, const Edge& rhs) {
        if (lhs.from != rhs.from) {
            return lhs.from < rhs.from;
        } else {
            return lhs.to < rhs.to;
        }
    });

    for (int i = 0; i < dup_edges.size(); i++) {
        if (graph.edges.empty()) {
            graph.edges.push_back(dup_edges[i]);
        } else {
            const auto &last_edge = graph.edges[graph.edges.size() - 1];
            if (dup_edges[i].from != last_edge.from || dup_edges[i].to != last_edge.to) {
                graph.edges.push_back(dup_edges[i]);
            }
        }
    }

    return graph;

}

Graph sparseGraph(int num_nodes, int edge_upper_bound) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> edge_idx(0, num_nodes-1);
    uniform_real_distribution<double> weight_dist(upper_weight, lower_weight);

    Graph graph;
    graph.num_vertices = num_nodes;

    vector<Edge> dup_edges;
    vector<bool> marked(num_nodes, false);
    for (int i = 0; i < num_nodes; i++) {
        // init
        for (int j = 0; j < num_nodes; j++) {
            marked[j] = false;
        }

       // set mark
       for (int j = 0; j < edge_upper_bound; j++) {
           marked[edge_idx(mt)] = true;
       }

       // set edge
       for (int j = 0; j < num_nodes; j++) {
           if (j != i && marked[j]) {
               dup_edges.push_back({std::min(i, j), std::max(i, j), weight_dist(mt)});
           }
       }
    }

    // dedup
    std::sort(dup_edges.begin(), dup_edges.end(), [](const Edge& lhs, const Edge& rhs) {
        if (lhs.from != rhs.from) {
            return lhs.from < rhs.from;
        } else {
            return lhs.to < rhs.to;
        }
    });

    for (int i = 0; i < dup_edges.size(); i++) {
        if (graph.edges.empty()) {
            graph.edges.push_back(dup_edges[i]);
        } else {
            const auto &last_edge = graph.edges[graph.edges.size() - 1];
            if (dup_edges[i].from != last_edge.from || dup_edges[i].to != last_edge.to) {
                graph.edges.push_back(dup_edges[i]);
            }
        }
    }

    return graph;
}

Graph randomGraph(int num_nodes, double prob_thre_edge) {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> edge_dist(0, 1);
    uniform_real_distribution<double> weight_dist(upper_weight, lower_weight);

    Graph graph;
    graph.num_vertices = num_nodes;

    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            if (edge_dist(mt) < prob_thre_edge) {
                graph.edges.push_back({i, j, weight_dist(mt)});
            }
        }
    }

    return graph;
}

int main(int argc, char *argv[]) {
    
//     Parse the arguement.
    if (argc != 4) {
        cerr << "graph_generator <type:[r|s|p]> <param...> "<< endl;
        cerr << "\t r (random): <num_nodes> <prob_thre_edge> "<< endl;
        cerr << "\t s (sparse): <num_nodes> <half degree upper bound> "<< endl;
        cerr << "\t p (power law): <num_nodes> <power degree: choose 1.5, 2.5, 3.5> "<< endl;
        return 1;
    }

    char opt = argv[1][0];

    Graph src_graph;
    char path[1024];
    switch(opt) {
        case 'r':
            src_graph = randomGraph(atoi(argv[2]), stod(string(argv[3])));
            sprintf(path, "./random_%s_%s", argv[2], argv[3]);
            break;
        case 's':
            src_graph = sparseGraph(atoi(argv[2]), atoi(argv[3]));
            sprintf(path, "./sparse_%s_%s", argv[2], argv[3]);
            break;
        case 'p':
            src_graph = powerLawGraph(atoi(argv[2]), -stod(string(argv[3])));
            sprintf(path, "./power_%s_%s", argv[2], argv[3]);
            break;
        default:
            cout << "Unknown graph type" << endl;
            return 1;
    }

    string filename(path);
    src_graph.saveGraph(filename);

    Graph dst_graph;
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




