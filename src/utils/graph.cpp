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

void Graph::loadGraph(const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "rb");
    size_t m;
    fread(&num_vertices, sizeof(num_vertices), 1, fp);
    fread(&m, sizeof(m), 1, fp);
    edges.resize(m);
    size_t i = 0;
    while (i < m) {
        i += fread(&edges[i], sizeof(Edge), m - i, fp);
    }
    fclose(fp);

}

void Graph::saveGraph(const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "wb");
    size_t m = edges.size(); // FIXME: not safe
    fwrite(&num_vertices, sizeof(num_vertices), 1, fp);
    fwrite(&m, sizeof(m), 1, fp);
    size_t i = 0;
    while (i < m) {
        i += fwrite(&edges[i], sizeof(Edge), m - i, fp);
    }
    fclose(fp);
}
