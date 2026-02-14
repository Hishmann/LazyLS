#pragma once

#include "pch.h"

#include <filesystem>

namespace fs = std::filesystem;

inline fs::path current;
 
std::optional<std::vector<std::string>> contents_list(fs::path input);