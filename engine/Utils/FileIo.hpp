#pragma once

#include <string_view>
#include <optional>

std::optional<std::string> tryLoadStringFromFile(std::string_view path);
