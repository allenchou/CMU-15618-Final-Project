add_library(graph graph.cpp disjoint_set.cpp)
add_executable(graph_generator graph_generator.cpp graph.cpp)
