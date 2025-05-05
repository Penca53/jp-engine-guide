#pragma once

#include <type_traits>

namespace ng {

/// @brief Concept that checks if type `T` is derived from type `U`.
/// @tparam T The potentially derived type.
/// @tparam U The base type.
template <class T, class U>
concept Derived = std::is_base_of_v<U, T>;

}  // namespace ng