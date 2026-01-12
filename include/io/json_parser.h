#pragma once
#include <string>

#include "io/input_parser.h"

namespace io {

InputSpec ParseJsonText(const std::string& text);
InputSpec ParseJsonFile(const std::string& path);

}  // namespace io
