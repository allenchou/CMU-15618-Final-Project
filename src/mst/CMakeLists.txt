
#find_package(Boost 1.71.0 REQUIRED filesystem)
#if(NOT Boost_FOUND)
#    message(FATAL_ERROR "Could not find boost!")
#endif()
#message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#message(STATUS "Boost_VERSION: ${Boost_VERSION}")

include_directories(
        ${PROJECT_SOURCE_DIR}/src/utils
#        ${Boost_INCLUDE_DIRS}
)

add_executable(kruskal_seq kruskal_seq.cpp)
target_link_libraries(kruskal_seq graph)

add_executable(kruskal_par_sort kruskal_par_sort.cpp)
target_link_libraries(kruskal_par_sort graph)

add_executable(kruskal_filter kruskal_filter.cpp)
target_link_libraries(kruskal_filter graph)

add_executable(kruskal_ref kruskal_ref.cpp)
target_link_libraries(kruskal_ref graph)
