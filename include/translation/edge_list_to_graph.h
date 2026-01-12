#pragma once
#include "graph/adj_list_graph.h"
#include "io/input_parser.h"

AdjListGraph EdgeListToGraph(const io::EdgeListProblem& problem);
