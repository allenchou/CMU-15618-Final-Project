/**
 * @file graph_generator.cpp
 * @brief Graph data generator.
 *
 * @author Xuren Zhou (xurenz)
 * @bug No known bugs.
 */

#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main(int argc, char *argv[]) {

    size_t num_nodes = 5000;
    double prob_thre_edge = 0.05;
    double upper_weight = 1.0;
    double lower_weight = 10.0;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> edge_dist(0, 1);
    uniform_real_distribution<double> weight_dist(upper_weight, lower_weight);

    string filename = "random_" + to_string(num_nodes) + "_" + to_string(prob_thre_edge) + ".txt";
    ofstream fout;
    fout.open(filename);
    fout << num_nodes << endl;
    for (size_t i = 0; i < num_nodes; i++) {
        for (size_t j = i + 1; j < num_nodes; j++) {
            if (edge_dist(mt) < prob_thre_edge) {
                fout << i << " " << j << " " << weight_dist(mt) << endl;
            }
        }
    }
    fout.close();

    return 0;
}




