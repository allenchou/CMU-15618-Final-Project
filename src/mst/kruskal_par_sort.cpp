//
// Created by Xuren Zhou on 11/17/19.
//
#include <algorithm>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include "disjoint_set.h"
#include "graph.h"

#define PAR_SORT_THRESHOLD 8192
#define NUM_PARTITIONS 16
#define OVERSAMPLING_RATIO 200

void parallelQuickSort(std::vector<Edge>::iterator begin,
                       std::vector<Edge>::iterator end) {
    auto cmp = [](const Edge &lhs, const Edge &rhs) {
        return lhs.weight < rhs.weight;
    };
    size_t dist = std::distance(begin, end);
    if (dist <= PAR_SORT_THRESHOLD) {
        sort(begin, end, cmp);
        return;
    }

    auto pivot = *std::next(begin, dist / 2); // FIXME: random chosen
    std::vector<Edge>::iterator iter[4];
    iter[0] = begin;
    iter[3] = end;

    iter[1] = std::partition(begin, end, [pivot](const Edge &edge) {
        return edge.weight < pivot.weight;
    });
    iter[2] = std::partition(iter[1], end, [pivot](const Edge &edge) {
        return pivot.weight >= edge.weight;
    });

    for (int i = 0; i < 2; i++) {
#pragma omp task
        parallelQuickSort(iter[2 * i], iter[2 * i + 1]);
    }
#pragma omp taskwait
}

void parallelEnumerationSort(const std::vector<Edge> &edges, const int i,
                             std::vector<Edge> *output) {
    int output_idx = 0;
    for (int j = 0; j < (int) edges.size(); j++) {
        if (edges[j].weight < edges[i].weight ||
            (edges[j].weight == edges[i].weight && j < i)) {
            output_idx += 1;
        }
    }
    (*output)[output_idx] = edges[i];
}

void count_bucket(const std::vector<double> &pivots, Edge *edges, const int len,
                  int *bucket) {
    for (int i = 0; i < len; ++i) {
        const int bucket_idx =
                std::upper_bound(pivots.begin(), pivots.end(),
                                 edges[i].weight) -
                pivots.begin();
        bucket[bucket_idx]++;
    }
}

void map_partition(const std::vector<double> &pivots, Edge *edges,
                   const int len, int *bucket_start_idx,
                   std::vector<Edge> *new_edges) {
    // Copy the start index for each bucket.
    std::vector<int> bucket_idxes(NUM_PARTITIONS);
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        bucket_idxes[i] = bucket_start_idx[i];
    }

    for (int i = 0; i < len; ++i) {
        const int bucket_idx =
                std::upper_bound(pivots.begin(), pivots.end(),
                                 edges[i].weight) -
                pivots.begin();
        (*new_edges)[bucket_idxes[bucket_idx]] = edges[i];
        bucket_idxes[bucket_idx]++;
    }
}

void ParallelSampleSort(std::vector<Edge> &edges) {
    const int num_edges = (int) edges.size();
    const int num_maps = NUM_PARTITIONS * NUM_PARTITIONS;
    const int partition_size =
            (num_edges + NUM_PARTITIONS - 1) / NUM_PARTITIONS;

    // Step 1: select the pivots.
    std::vector<double> all_pivots(NUM_PARTITIONS * OVERSAMPLING_RATIO);
    for (int i = 0; i < NUM_PARTITIONS * OVERSAMPLING_RATIO; ++i) {
        all_pivots[i] = edges[rand() % num_edges].weight;
    }
    std::sort(all_pivots.begin(), all_pivots.end());

    std::vector<double> pivots(NUM_PARTITIONS - 1);
    for (int i = 0; i < NUM_PARTITIONS - 1; ++i) {
        pivots[i] = all_pivots[(i + 1) * OVERSAMPLING_RATIO - 1];
    }

    // Step 2: calculate the size of each bucket for each partition.
    std::vector<int> bucket(num_maps);
#pragma omp parallel for
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        const int start = i * partition_size;
        const int end = std::min(start + partition_size, num_edges);
        count_bucket(pivots, &edges[start], end - start,
                     &bucket[i * NUM_PARTITIONS]);
    }

    // Step 3: generate the map for each partition.
    std::vector<int> temp(num_maps);
    std::vector<int> bucket_start_idx(num_maps);
    for (int i = 1; i < num_maps; ++i) {
        temp[i] = temp[i - 1] +
                  bucket[((i - 1) % NUM_PARTITIONS) * NUM_PARTITIONS +
                         (i - 1) / NUM_PARTITIONS];
    }
    // Transpose the matrix.
    for (int i = 0; i < num_maps; ++i) {
        bucket_start_idx[i] = temp[(i % NUM_PARTITIONS) * NUM_PARTITIONS +
                                   i / NUM_PARTITIONS];
    }

    // Step 4: map into separate partition.
    std::vector<Edge> new_edges(num_edges);
#pragma omp parallel for
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        const int start = i * partition_size;
        const int end = std::min(start + partition_size, num_edges);
        map_partition(pivots, &edges[start], end - start,
                      &bucket_start_idx[i * NUM_PARTITIONS], &new_edges);
    }

    // Step 5: sort within each partition.
    auto cmp = [](const Edge &lhs, const Edge &rhs) {
        return lhs.weight < rhs.weight;
    };
#pragma omp parallel for
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        const int end_idx = (i == NUM_PARTITIONS - 1 ? num_edges
                                                     : bucket_start_idx[i + 1]);
        std::sort(&new_edges[bucket_start_idx[i]], &new_edges[end_idx], cmp);
    }

    edges = std::move(new_edges);
}

Graph kruskalMSTSequential(const Graph &graph, std::string sort_algo) {
    struct timeval start, end;
    double duration;
    double total = 0;

    // Copy and sort the edges by weight.
    std::vector<Edge> edges = graph.edges;
    std::cout << "Length of edges: " << edges.size() << std::endl;
    gettimeofday(&start, nullptr);

    if (sort_algo == "QS") {
#pragma omp parallel
#pragma omp single nowait
        parallelQuickSort(edges.begin(), edges.end());
    } else if (sort_algo == "ES") {
        const int num_edges = (int) edges.size();
        std::vector<Edge> sorted_edges(num_edges);
#pragma omp parallel for
        for (int i = 0; i < num_edges; i++) {
            parallelEnumerationSort(edges, i, &sorted_edges);
        }
        edges = std::move(sorted_edges);
    } else if (sort_algo == "SS") {
        ParallelSampleSort(edges);
    }

    gettimeofday(&end, nullptr);
    duration = (double) ((end.tv_sec - start.tv_sec) * 1000000u +
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
    std::cout << "Merging time: " << duration << " s." << std::endl;

    total += duration;
    std::cout << "total time: " << total << " s." << std::endl;
    std::cout << "sort percent: " << 1 - duration / total << std::endl;
    return {graph.num_vertices, mst_edges};
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr
                << "Invalid command: bin/kruskal_par_sort <data_path> <QS|ES|SS>"
                  << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::string sort_algo = argv[2];
    std::cout << "File name: " << filename << " Sort algorithm: " << sort_algo
              << std::endl;
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
    Graph mst = kruskalMSTSequential(graph, sort_algo);

    // Save the result.
    std::string output_filename = filename + "." + sort_algo + ".output";
    std::cout << "Saving MST to " << output_filename << "." << std::endl;
    mst.saveGraph(output_filename);

    return 0;
}
