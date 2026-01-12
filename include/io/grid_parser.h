#pragma once
#include "io/input_parser.h"

namespace io {

GridProblem ParseGridProblem(const nlohmann::json& problem);

}  // namespace io
