#pragma once

#include <string>
#include <vector>

namespace String_functions
{
std::string sub_string(const std::string &str, size_t begin, size_t end);
std::vector<std::string> split(const std::string &str, char separator);

} // namespace String_functions
