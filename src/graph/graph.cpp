//
// Created by Xuren Zhou on 11/12/19.
//

#include <iostream>
#include "graph.h"

void Graph::printGraph() {
    std::cout << "num_nodes: " << num_vertices << std::endl;
    for (const Edge &e : edges) {
        std::cout << e.from << '-' << e.to << ',' << e.weight << std::endl;
    }
}
