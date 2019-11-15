//
// Created by Xuren Zhou on 11/12/19.
//

#ifndef MST_DISJOINT_SET_H
#define MST_DISJOINT_SET_H


#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int);
    int findSet(int);
    void unionSet(int, int);
    bool belongSameSet(int, int);
private:
    int num_nodes;
    std::vector<int> ranks;
    std::vector<int> parents;
};


#endif //MST_DISJOINT_SET_H
