//
// Created by Xuren Zhou on 11/12/19.
//

#include <cassert>
#include "disjoint_set.h"

DisjointSet::DisjointSet(const int n): num_nodes(n), ranks(n, 1) {
    parents.reserve(n);
    for (int i = 0; i < num_nodes; ++i) {
        parents.push_back(i);
    }
}

int DisjointSet::findSet(const int node) {
    if (parents[node] != node) {
        parents[node] = findSet(parents[node]);
    }
    return parents[node];
}

void DisjointSet::unionSet(const int node1, const int node2) {
    const int p_a = findSet(node1);
    const int p_b = findSet(node2);

    // If two nodes belong to same set, return.
    if (p_a == p_b) {
        return;
    }

    // Union by rank.
    if (ranks[p_a] < ranks[p_b]) {
        parents[p_a] = p_b;
    } else if (ranks[p_a] > ranks[p_b]) {
        parents[p_b] = p_a;
    } else {
        parents[p_a] = p_b;
        ranks[p_b]++;
    }
}

bool DisjointSet::belongSameSet(const int node1, const int node2) {
    assert(0 <= node1 && node1 < num_nodes);
    assert(0 <= node2 && node2 < num_nodes);
    return findSet(node1) == findSet(node2);
}
