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
    int m;
    fread(&num_vertices, sizeof(num_vertices), 1, fp);
    fread(&m, sizeof(m), 1, fp);
    edges.resize(m);
    fread(&edges[0], sizeof(Edge), m, fp);
    fclose(fp);

}

void Graph::saveGraph(const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "wb");
    int m = edges.size(); // FIXME: not safe
    fwrite(&num_vertices, sizeof(num_vertices), 1, fp);
    fwrite(&m, sizeof(m), 1, fp);
    edges.resize(m);
    fwrite(&edges[0], sizeof(Edge), m, fp);
    fclose(fp);
}
