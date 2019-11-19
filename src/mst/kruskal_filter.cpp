//
// Created by Xuren Zhou on 11/17/19.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include "disjoint_set.h"
#include "graph.h"

#define SEQ_THRESHOLD 8192

using namespace std;

vector<Edge> kruskal(vector<Edge>::iterator begin, vector<Edge>::iterator end, DisjointSet& ds) {
    // sort edges
    sort(begin, end, [](const Edge& lhs, const Edge& rhs){ return lhs.weight < rhs.weight; });
    // Kruskal algorithm.
    std::vector<Edge> mst_edges;
    for (auto iter = begin; iter != end; ++iter) {
        if (!ds.belongSameSet(iter->from, iter->to)) {
            ds.unionSet(iter->from, iter->to);
            mst_edges.push_back(*iter);
        }
    }

    return mst_edges;
}

vector<Edge>::iterator filter(vector<Edge>::iterator begin, vector<Edge>::iterator end, DisjointSet& ds) {
    return partition(begin, end, [&ds](const Edge& edge) { return ds.belongSameSet(edge.from, edge.to); });
}

vector<Edge> filterKruskal(vector<Edge>::iterator begin, vector<Edge>::iterator end, DisjointSet& ds) {
    size_t dist = distance(begin, end);
    if (dist <= SEQ_THRESHOLD) {
        return kruskal(begin, end, ds);
    }
    auto pivot = *std::next(begin, dist / 2); // FIXME: random chosen
    auto middle = std::partition(begin, end, [pivot](const Edge& edge) {return edge.weight <= pivot.weight; });

    vector<Edge> small_edges, large_edges;
    vector<Edge>::iterator large_begin;

//#pragma omp task depend(out:small_edges) depend(out:ds)
    small_edges = filterKruskal(begin, middle, ds);

//#pragma omp task depend(out:large_begin) depend(in:ds)
    large_begin = filter(middle, end, ds);
//#pragma omp taskwait

//#pragma omp task depend(in:large_begin)
    large_edges = filterKruskal(large_begin, end, ds);

    small_edges.insert(small_edges.end(), large_edges.begin(), large_edges.end());
    return small_edges;
}

Graph kruskalMST(const Graph &graph) {
    DisjointSet ds(graph.num_vertices);
    std::vector<Edge> edges = graph.edges;
    struct timeval start, end;
    double duration;

    gettimeofday(&start, nullptr);
    vector<Edge> mst_edges;

//#pragma omp parallel
//#pragma omp single nowait
    mst_edges = filterKruskal(edges.begin(), edges.end(), ds);

    gettimeofday(&end, nullptr);
    duration = (double)((end.tv_sec  - start.tv_sec) * 1000000u +
                        end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "filter-kruskal time: " << duration << " s." << std::endl;

    std::cout << "#nodes: " << graph.num_vertices << " #edges: " << mst_edges.size() << endl;

    return {graph.num_vertices, mst_edges};
}

int main() {
    std::string filename = "/Users/allenchou/Documents/CMU_courses/15618/project/CMU-15618-Final-Project/data/random_20000_0.2.txt";
    std::cout << "File name: " << filename << std::endl;
    struct timeval start, end;
    double duration;

    gettimeofday(&start, nullptr);
    Graph graph;
    graph.loadGraph(filename);
    gettimeofday(&end, nullptr);
    duration = (double)((end.tv_sec  - start.tv_sec) * 1000000u +
                        end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "loading time: " << duration << " s." << std::endl;

    // Execute the algorithm and print the MST.
    Graph mst = kruskalMST(graph);
//    mst.printGraph();

    return 0;
}