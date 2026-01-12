cmake_minimum_required(VERSION 3.12)
# project name
project(cpp_graph_solver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(graph
    src/graph/adj_list_graph.cpp
)
target_include_directories(graph PUBLIC include)

add_library(ds
    src/ds/union_find.cpp
)
target_include_directories(ds PUBLIC include)

add_library(utils
    src/utils/reverse_graph.cpp
)
target_include_directories(utils PUBLIC include)
target_link_libraries(utils PUBLIC graph)

add_library(algorithms
    src/algorithms/flow/max_flow.cpp
    src/algorithms/shortest_path/bfs.cpp
    src/algorithms/shortest_path/dijkstra.cpp
    src/algorithms/shortest_path/dfs.cpp
    src/algorithms/shortest_path/astar.cpp
    src/algorithms/mst/mst.cpp
    src/algorithms/connectivity/connected_components.cpp
    src/algorithms/connectivity/cycle_detection.cpp
    src/algorithms/dag/topo_sort.cpp
    src/algorithms/dag/is_dag.cpp
)
target_include_directories(algorithms PUBLIC include)
target_link_libraries(algorithms PUBLIC graph ds)

add_library(translation
    src/translation/grid_to_graph.cpp
    src/translation/edge_list_to_graph.cpp
    src/translation/task_to_flow.cpp
)
target_include_directories(translation PUBLIC include)
target_link_libraries(translation PUBLIC graph nlohmann_json::nlohmann_json)

add_library(io
    src/io/json_parser.cpp
    src/io/grid_parser.cpp
)
target_include_directories(io PUBLIC include)
target_link_libraries(io PUBLIC translation nlohmann_json::nlohmann_json)

add_executable(graph_solver
    src/main.cpp
)
target_link_libraries(graph_solver PRIVATE io algorithms utils)
target_include_directories(graph_solver PRIVATE .)

install(TARGETS graph_solver RUNTIME DESTINATION bin)

# test files
add_executable(max_flow_test
    test/max_flow_test.cpp
)
target_link_libraries(max_flow_test PRIVATE algorithms)
target_include_directories(max_flow_test PRIVATE .)

add_executable(shortest_path_test
    test/shortest_path_test.cpp
)
target_link_libraries(shortest_path_test PRIVATE algorithms)
target_include_directories(shortest_path_test PRIVATE .)

add_executable(connected_components_test
    test/connected_components_test.cpp
)
target_link_libraries(connected_components_test PRIVATE algorithms)
target_include_directories(connected_components_test PRIVATE .)

add_executable(cycle_detection_test
    test/cycle_detection_test.cpp
)
target_link_libraries(cycle_detection_test PRIVATE algorithms)
target_include_directories(cycle_detection_test PRIVATE .)

add_executable(mst_test
    test/mst_test.cpp
)
target_link_libraries(mst_test PRIVATE algorithms)
target_include_directories(mst_test PRIVATE .)

add_executable(topo_sort_test
    test/topo_sort_test.cpp
)
target_link_libraries(topo_sort_test PRIVATE algorithms)
target_include_directories(topo_sort_test PRIVATE .)

add_executable(reverse_graph_test
    test/reverse_graph_test.cpp
)
target_link_libraries(reverse_graph_test PRIVATE utils)
target_include_directories(reverse_graph_test PRIVATE .)

add_executable(union_find_test
    test/union_find_test.cpp
)
target_link_libraries(union_find_test PRIVATE ds)
target_include_directories(union_find_test PRIVATE .)
