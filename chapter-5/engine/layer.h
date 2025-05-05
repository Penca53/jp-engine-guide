#pragma once

#include <cstdint>

namespace ng {

/// @brief Defines bitmask flags for rendering layers, allowing selective rendering of objects.
enum class Layer : uint64_t {  // NOLINT
  /// @brief The default rendering layer.
  kDefault = 1ULL << 0ULL,
  /// @brief The user interface rendering layer.
  kUI = 1ULL << 1ULL,
};

}  // namespace ng
