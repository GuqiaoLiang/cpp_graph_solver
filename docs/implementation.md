# Graph Solver CLI --- Implementation Plan (Used for codex)

A modular command-line solver for graph and optimization problems.\
Input and output are both JSON. Multiple analysis queries can be
executed on the same problem instance.

------------------------------------------------------------------------

## ✅ Features (V1 Target)

-   CLI tool: `input.json → solver → output.json`
-   JSON-only input and output
-   Multiple problem modes
-   Multiple queries per problem
-   Modular architecture: parsing → translation → algorithms → output

Supported problem types:

-   Graph (edge list)
    -   Connected components
    -   Cycle detection
    -   MST (Prim / Kruskal)
    -   Shortest path (BFS / Dijkstra)
-   Grid map
    -   Shortest path (BFS / A\*)
-   Flow network
    -   Max flow (Dinic)

------------------------------------------------------------------------

## 🧱 Overall Architecture

    CLI
     └─ Input Loader
         └─ JSON Parser
             └─ Mode Dispatcher
                 └─ Translation Layer
                     └─ Algorithm Dispatcher
                         └─ Result Collector
                             └─ JSON Writer

Design principles:

-   Algorithms never access JSON
-   Translation layer converts problem → algorithm data structures
-   Same problem can serve multiple queries

------------------------------------------------------------------------

## ▶ CLI Usage

``` bash
solver input.json output.json [options]
```

Optional flags (future):

``` bash
--verbose
--override_algo kruskal
--time_limit 2s
```

Example inputs/outputs live in:

- `docs/examples/edge_list_input.json`
- `docs/examples/edge_list_output.json`
- `docs/examples/grid_input.json`
- `docs/examples/grid_output.json`
- `docs/examples/flow_input.json`
- `docs/examples/flow_output.json`

------------------------------------------------------------------------

## 📄 Input JSON Format (Unified)

### Top-level Schema

``` json
{
  "mode": "...",
  "problem": { ... },
  "queries": [ ... ]
}
```

-   `mode` determines translation pipeline
-   `problem` describes the instance
-   `queries` specifies multiple computations on the same instance

------------------------------------------------------------------------

## 🧩 Mode: `edge_list` (Graph Algorithms)

### Example

``` json
{
  "mode": "edge_list",
  "problem": {
    "num_vertices": 6,
    "directed": false,
    "edges": [
      [0,1,3],
      [1,2,2],
      [2,3,4],
      [3,1,1],
      [4,5,7]
    ]
  },
  "queries": [
    { "type": "connected_components" },
    { "type": "contains_cycle" },
    { "type": "mst", "method": "kruskal" },
    { "type": "shortest_path", "source": 0, "target": 3 }
  ]
}
```

### Supported Queries

#### Connected Components

``` json
{ "type": "connected_components" }
```

Output:

``` json
{
  "count": 2,
  "components": [[0,1,2,3],[4,5]]
}
```

------------------------------------------------------------------------

#### Cycle Detection

``` json
{ "type": "contains_cycle" }
```

Output:

``` json
{ "has_cycle": true }
```

------------------------------------------------------------------------

#### Minimum Spanning Tree

``` json
{ "type": "mst", "method": "kruskal" }
```

or

``` json
{ "type": "mst", "method": "prim", "start": 0 }
```

Output:

``` json
{
  "total_weight": 13,
  "edges": [[1,3,1],[1,2,2],[0,1,3],[4,5,7]]
}
```

------------------------------------------------------------------------

#### Shortest Path

``` json
{ "type": "shortest_path", "source": 0, "target": 3 }
```

or all distances from source:

``` json
{ "type": "shortest_path", "source": 0 }
```

Output:

``` json
{
  "distances": [0,3,5,6,null,null],
  "path_to_target": [0,1,3]
}
```

------------------------------------------------------------------------

## 🧭 Mode: `grid` (Pathfinding)

### Example

``` json
{
  "mode": "grid",
  "problem": {
    "rows": 5,
    "cols": 5,
    "map": [
      "..#..",
      ".#.#.",
      "...#T",
      "##...",
      "S...."
    ],
    "start": [4,0],
    "target": [2,4],
    "allow_diagonal": false
  },
  "queries": [
    { "type": "shortest_path", "method": "bfs" }
  ]
}
```

Translation layer:

-   Grid → implicit neighbors or Graph

Algorithms:

-   BFS
-   A\*

------------------------------------------------------------------------

## 🌊 Mode: `flow` (Max Flow)

### Example

``` json
{
  "mode": "flow",
  "problem": {
    "num_nodes": 6,
    "source": 0,
    "sink": 5,
    "edges": [
      [0,1,10],
      [1,3,5],
      [3,5,8],
      [0,2,7],
      [2,3,3]
    ]
  },
  "queries": [
    { "type": "max_flow", "method": "dinic" }
  ]
}
```

Translation:

-   JSON → Dinic residual network

Algorithm:

-   Dinic max flow

Output:

``` json
{
  "max_flow": 15
}
```

------------------------------------------------------------------------

## 📤 Output JSON Format

### Top-level Output

``` json
{
  "mode": "edge_list",
  "results": [
    {
      "query": { "type": "connected_components" },
      "result": {
        "count": 2,
        "components": [[0,1,2,3],[4,5]]
      }
    },
    {
      "query": { "type": "mst", "method": "kruskal" },
      "result": {
        "total_weight": 13,
        "edges": [[1,3,1],[1,2,2],[0,1,3],[4,5,7]]
      }
    }
  ],
  "stats": {
    "time_ms": 4.2
  },
  "meta": {
    "solver_version": "v1.0"
  }
}
```

Each query produces an independent result block.

------------------------------------------------------------------------

## 🧩 Internal Module Layout

    include/
      graph/
      ds/
      algorithms/
        mst/
        shortest_path/
        flow/
      io/
      translation/

    src/
      graph/
      ds/
      algorithms/
      io/
      translation/

    test/
      graph_tests.cpp
      grid_tests.cpp
      flow_tests.cpp

------------------------------------------------------------------------

## 🔁 Solver Execution Flow

    1. Read JSON
    2. Parse mode / problem / queries
    3. Translate problem into algorithm data structures
    4. For each query:
         dispatch algorithm
         collect result
    5. Write output JSON

------------------------------------------------------------------------

## 🛠 Development Roadmap

### Phase 0 --- CLI Skeleton

-   main.cpp
-   JSON read/write
-   echo input → output

------------------------------------------------------------------------

### Phase 1 --- edge_list + MST

-   Graph builder
-   Kruskal / Prim
-   Multi-query support

------------------------------------------------------------------------

### Phase 2 --- grid + BFS

-   Grid parser
-   Grid → Graph or implicit BFS

------------------------------------------------------------------------

### Phase 3 --- flow + Dinic

-   Flow parser
-   Dinic integration

------------------------------------------------------------------------

### Phase 4 --- Engineering Enhancements

-   runtime statistics
-   batch mode
-   graph export (Graphviz)

------------------------------------------------------------------------

## 🎯 Design Philosophy

-   Algorithm layer is independent of input format
-   Translation layer isolates modeling decisions
-   CLI-first design for automation and experiments
-   JSON as problem specification language

------------------------------------------------------------------------

## 📌 Future Extensions

-   Min-cost max-flow
-   Bipartite matching
-   Task scheduling models
-   LP/MIP backend integration
-   Visualization via trace export

------------------------------------------------------------------------

## ✅ Project Value

This project is not just an algorithm demo, but a:

-   graph analysis tool
-   optimization solver prototype
-   experiment framework for research

Suitable for:

-   systems projects
-   optimization research
-   AI infrastructure exploration
